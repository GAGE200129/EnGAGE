#include "pch.hpp"
#include "ECS.hpp"

#include "Script.hpp"

#define SET_BIT(x, loc) x |= 1 << loc
#define UNSET_BIT(x, loc) x &= ~(1 << loc)

namespace Core::ECS
{
	//Entity
	static unsigned int gEntityCounter;
	static unsigned int gLivingEntities;
	static Arr<EntitySignature, MAX_ENTITIES> gEntitiesSignatures;
	static Set<unsigned int> gEntitiesMarkedForRemoval;

	//Component
	static Map<ComponentType, ComponentArray> gComponentArrays;

	//System
	static Map<SystemType, System>  gSystems;

	static EntitySignature& searchEntity(unsigned int id);
	static void removeComponentInternal(ComponentArray& componentArray, EntitySignature& entity);
	static void updateSystems(EntitySignature& entity);
	static void* constructComponent(unsigned int entity, ComponentType type, const void* extraData);


	void init()
	{
		gEntityCounter = 0;
		gLivingEntities = 0;

		//Init components
		for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
		{
			ComponentData data = getComponentData((ComponentType)i);
			ComponentArray arr;
			arr.size = data.size;
			arr.count = 0;
			arr.data = createScope<char[]>(MAX_COMPONENT_ARRAY_BUFFER_SIZE);
			gComponentArrays.insert({ (ComponentType)i, std::move(arr) });
		}

		//Renderer system
		unsigned int signature = 0;
		SET_BIT(signature, (unsigned int)ComponentType::TRANSFORM);
		SET_BIT(signature, (unsigned int)ComponentType::MODEL_RENDERER);
		gSystems[SystemType::RENDERER].signature = signature;


		signature = 0;
		SET_BIT(signature, (unsigned int)ComponentType::SCRIPT);
		gSystems[SystemType::SCRIPTING].signature = signature;

		signature = 0;
		SET_BIT(signature, (unsigned int)ComponentType::RIGID_BODY);
		SET_BIT(signature, (unsigned int)ComponentType::TRANSFORM);
		gSystems[SystemType::PHYSICS].signature = signature;

	}

	void shutdown()
	{
		gEntityCounter = 0;
		gLivingEntities = 0;

		for (auto& [type, system] : gSystems)
		{
			system.entities.clear();
		}

		for (auto& [type, componentArr] : gComponentArrays)
		{
			componentArr.count = 0;
			componentArr.entityToIndex.clear();
		}
	}

	void updateRemovedEntities()
	{
		for (const auto& entity : gEntitiesMarkedForRemoval)
		{
			removeEntity(entity);
		}
		gEntitiesMarkedForRemoval.clear();
	}

	unsigned int createEntity()
	{
		EN_ASSERT(gLivingEntities < MAX_ENTITIES, "MAX_ENTITIES = {}", MAX_ENTITIES);

		EntitySignature entity;
		entity.id = gEntityCounter;
		entity.signature = 0;
		gEntitiesSignatures[gLivingEntities] = entity;
		gEntityCounter++;
		gLivingEntities++;
		return entity.id;
	}
	void removeEntity(unsigned int entity)
	{
		auto& entitySignature = searchEntity(entity);

		//Update component arrays
		for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
		{
			removeComponent(entity, (ComponentType)i);
		}
		//Clear signature
		entitySignature.signature = 0;

		//Signal each systems
		updateSystems(entitySignature);

		//Remove entity	
		for (unsigned int i = 0; i < gLivingEntities; i++)
		{
			if (gEntitiesSignatures[i].id == entitySignature.id)
			{
				if (i == (gLivingEntities - 1))
				{
					gLivingEntities--;
					break;
				}
				else
				{
					gEntitiesSignatures[i] = gEntitiesSignatures[gLivingEntities - 1];
					gLivingEntities--;
					break;
				}
			}
		}
	}

	void markForRemove(unsigned int entity)
	{
		gEntitiesMarkedForRemoval.insert(entity);
	}

	const Arr<EntitySignature, MAX_ENTITIES>& getEntitySignatures()
	{
		return gEntitiesSignatures;
	}

	unsigned int getEntityCount()
	{
		return gLivingEntities;
	}

	void* addComponent(unsigned int entity, ComponentType type)
	{
		ComponentData data = getComponentData(type);
		Scope<char[]> extraData = createScope<char[]>(data.size);
		initComponent((ComponentHeader*)extraData.get(), type);

		return constructComponent(entity, type, extraData.get());
	}

	void removeComponent(unsigned int entity, ComponentType type)
	{
		auto& entitySignature = searchEntity(entity);
	
		destroyComponent((ComponentHeader*)ECS::getComponent(entity, type), type);

		//Update component arrays
		removeComponentInternal(gComponentArrays[type], entitySignature);

		//Update entity's signature
		UNSET_BIT(entitySignature.signature, (unsigned int)type);

		//Signal each systems
		updateSystems(entitySignature);
	}

	void* getComponent(unsigned int entity, ComponentType type)
	{
		ComponentArray& componentArray = gComponentArrays[type];

		if (componentArray.entityToIndex.find(entity) != componentArray.entityToIndex.end())
		{
			return componentArray.data.get() + componentArray.size * componentArray.entityToIndex.at(entity);
		}


		return nullptr;

	}

	System& getSystem(SystemType type)
	{
		return gSystems[type];
	}

	unsigned int getComponentArrayMemorySize(ComponentType type)
	{
		ComponentArray* arr = &gComponentArrays[type];
		return arr->size * arr->count;
	}

	

	EntitySignature& searchEntity(unsigned int id)
	{
		//Search for entity in entity arr
		for (unsigned int i = 0; i < gLivingEntities; i++)
		{
			if (gEntitiesSignatures[i].id == id)
			{
				return gEntitiesSignatures[i];
				break;
			}
		}
		EN_ASSERT(false, "Entity not found");
	}

	void removeComponentInternal(ComponentArray& componentArray, EntitySignature& entity)
	{
		ComponentHeader* header;
		char* offset;
		char* end;
		for (unsigned int i = 0; i < componentArray.count; i++)
		{
			offset = componentArray.data.get() + componentArray.size * i;
			header = (ComponentHeader*)offset;
			if (header->entity == entity.id) //Entity id matched, remove current i component
			{
				//End of arr, ignore it and decrease arr count by 1
				if (i == (componentArray.count - 1))
				{
					componentArray.count--;
					componentArray.entityToIndex.erase(entity.id);
					break;
				}
				else //Else copy end of arr to removed slot
				{
					end = componentArray.data.get() + componentArray.size * (componentArray.count - 1);
					memcpy(offset, end, componentArray.size);
					componentArray.count--;

					unsigned int entityOfRemovedIndex = ((ComponentHeader*)end)->entity;
					componentArray.entityToIndex[entityOfRemovedIndex] = i;
					break;
				}
			}
		}
	}

	void updateSystems(EntitySignature& entity)
	{
		for (auto& pair : gSystems)
		{
			System& system = pair.second;
			if ((system.signature & entity.signature) != system.signature) // If signature doesn't match, loop through all entities and remove them
			{
				system.entities.erase(entity.id);
			}
			else if ((system.signature & entity.signature) == system.signature)// Signature match, add to entity list
			{
				system.entities.insert(entity.id);
			}
		}
	}

	static void* constructComponent(unsigned int entity, ComponentType type, const void* extraData)
	{
		auto& entitySignature = searchEntity(entity);

		//Update component arrays
		ComponentArray* pComponentArray;
		ComponentHeader* header;

		pComponentArray = &gComponentArrays[type];

		//Search array to check if this entity already has that component type
		for (unsigned int i = 0; i < pComponentArray->count; i++)
		{
			char* offset = pComponentArray->data.get() + pComponentArray->size * i;
			header = (ComponentHeader*)offset;
			if (header->entity == entity)
			{
				EN_WARN("Each entity can only have 1 component");
				return nullptr;
			}
		}
		ComponentData componentData = getComponentData(type);
		EN_ASSERT((pComponentArray->count * pComponentArray->size + componentData.size) < MAX_COMPONENT_ARRAY_BUFFER_SIZE, "Component array is full");
		//Copy data to end of array
		char* lastMemLoc = pComponentArray->data.get() + pComponentArray->count * pComponentArray->size;
		memcpy(lastMemLoc, extraData, pComponentArray->size);
		header = (ComponentHeader*)(lastMemLoc); //extract header
		header->entity = entity; // now this component has this entity as parent
		header->type = type;
		pComponentArray->count++;
		pComponentArray->entityToIndex.insert({ entity, pComponentArray->count - 1 });



		//Update entity's signature
		SET_BIT(entitySignature.signature, (unsigned int)type);

		//Signal each systems
		updateSystems(entitySignature);

		return pComponentArray->data.get() + (pComponentArray->count - 1) * pComponentArray->size;
	}
}







