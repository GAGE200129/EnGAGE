#include "pch.hpp"
#include "ECS.hpp"

#include "Core/LuaHostFunctions.hpp"


#define SET_BIT(x, loc) x |= 1 << loc
#define UNSET_BIT(x, loc) x &= ~(1 << loc)

namespace Core::ECS
{
	//Entity
	static UInt64 gEntityCounter;
	static UInt64 gLivingEntities;
	static Arr<EntitySignature, MAX_ENTITIES> gEntitiesSignatures;
	static Set<UInt64> gEntitiesMarkedForRemoval;

	//Component
	static OrderedMap<ComponentType, ComponentArray> gComponentArrays;

	//System
	static OrderedMap<SystemType, System>  gSystems;

	static EntitySignature* searchEntity(UInt64 id);
	static void removeComponentInternal(ComponentArray& componentArray, EntitySignature* entity);
	static void updateSystems(EntitySignature* entity);
	static void* constructComponent(UInt64 entity, ComponentType type, const void* extraData);

	void init()
	{
		gEntityCounter = 0;
		gLivingEntities = 0;

		//Init components
		for (UInt32 i = 0; i < (UInt32)ComponentType::COUNT; i++)
		{
			ComponentHint data = getComponentHint((ComponentType)i);
			ComponentArray arr;
			arr.size = data.size;
			arr.count = 0;
			arr.data = createScope<Byte[]>(MAX_COMPONENT_ARRAY_BUFFER_SIZE);
			gComponentArrays.insert({ (ComponentType)i, std::move(arr) });
		}

		//Renderer system
		UInt32 signature = 0;
		SET_BIT(signature, (UInt32)ComponentType::TRANSFORM);
		SET_BIT(signature, (UInt32)ComponentType::MODEL_RENDERER);
		gSystems[SystemType::RENDERER].signature = signature;


		signature = 0;
		SET_BIT(signature, (UInt32)ComponentType::SCRIPT);
		gSystems[SystemType::SCRIPTING].signature = signature;

		signature = 0;
		SET_BIT(signature, (UInt32)ComponentType::RIGID_BODY);
		SET_BIT(signature, (UInt32)ComponentType::TRANSFORM);
		gSystems[SystemType::PHYSICS].signature = signature;

		signature = 0;
		SET_BIT(signature, (UInt32)ComponentType::DIRECTIONAL_LIGHT);
		gSystems[SystemType::DIRECTIONAL].signature = signature;

		signature = 0;
		SET_BIT(signature, (UInt32)ComponentType::POINT_LIGHT);
		SET_BIT(signature, (UInt32)ComponentType::TRANSFORM);
		gSystems[SystemType::POINT].signature = signature;
	}

	void clear()
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

	UInt64 createEntity()
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
	void removeEntity(UInt64 entity)
	{
		auto entitySignature = searchEntity(entity);

		//Update component arrays
		for (UInt32 i = 0; i < (UInt32)ComponentType::COUNT; i++)
		{
			removeComponent(entity, (ComponentType)i);
		}
		//Clear signature
		entitySignature->signature = 0;

		//Signal each systems
		updateSystems(entitySignature);

		//Remove entity	
		for (UInt64 i = 0; i < gLivingEntities; i++)
		{
			if (gEntitiesSignatures[i].id == entitySignature->id)
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

	void markForRemove(UInt64 entity)
	{
		gEntitiesMarkedForRemoval.insert(entity);
	}

	const Arr<EntitySignature, MAX_ENTITIES>& getEntitySignatures()
	{
		return gEntitiesSignatures;
	}

	UInt64 getEntityCount()
	{
		return gLivingEntities;
	}

	void* addComponent(UInt64 entity, ComponentType type)
	{
		ComponentHint data = getComponentHint(type);
		Scope<Byte[]> extraData = createScope<Byte[]>(data.size);
		ComponentHeader* pHeader = (ComponentHeader*)extraData.get();
		pHeader->entity = entity;
		pHeader->type = type;
		initComponent((ComponentHeader*)extraData.get(), type);

		return constructComponent(entity, type, extraData.get());
	}

	void removeComponent(UInt64 entity, ComponentType type)
	{
		auto entitySignature = searchEntity(entity);
	
		auto* component = (ComponentHeader*)ECS::getComponent(entity, type);
		if (component)
		{
			auto destroyFn = component->Destroy;
			if(destroyFn)
				destroyFn(component);
		}

		//Update component arrays
		removeComponentInternal(gComponentArrays[type], entitySignature);

		//Update entity's signature
		UNSET_BIT(entitySignature->signature, (UInt32)type);

		//Signal each systems
		updateSystems(entitySignature);
	}

	void* getComponent(UInt64 entity, ComponentType type)
	{
		ComponentArray& componentArray = gComponentArrays[type];

		if (componentArray.entityToIndex.find(entity) != componentArray.entityToIndex.end())
		{
			return componentArray.data.get() + componentArray.size * componentArray.entityToIndex.at(entity);
		}
		return nullptr;
	}

	int luaCreateEntity(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 0);
		lua_pushinteger(L, ECS::createEntity());
		return 1;
	}

	int luaMarkForRemove(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 1);
		CHECK_ARG(L, 1, LUA_TNUMBER);

		unsigned int entity = (unsigned int)lua_tointeger(L, 1);
		ECS::markForRemove(entity);
		return 0;
	}

	int luaAddComponent(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 2);
		CHECK_ARG(L, 1, LUA_TNUMBER);// Entity id
		CHECK_ARG(L, 2, LUA_TNUMBER);// Type

		unsigned int entityID = (unsigned int)lua_tointeger(L, 1);
		ComponentType componentType = (ComponentType)lua_tointeger(L, 2);

		void* data = ECS::addComponent(entityID, componentType);
		lua_pushlightuserdata(L, data);
		return 1;
	}

	int luaGetComponent(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 2);
		CHECK_ARG(L, 1, LUA_TNUMBER);// Entity id
		CHECK_ARG(L, 2, LUA_TNUMBER);// Type

		unsigned int entityID = (unsigned int)lua_tointeger(L, 1);
		ComponentType componentType = (ComponentType)lua_tointeger(L, 2);

		void* component = ECS::getComponent(entityID, componentType);

		if (component == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushlightuserdata(L, component);
		return 1;
	}

	System& getSystem(SystemType type)
	{
		return gSystems[type];
	}

	UInt64 getComponentArrayMemorySize(ComponentType type)
	{
		ComponentArray* arr = &gComponentArrays[type];
		return arr->size * arr->count;
	}

	

	EntitySignature* searchEntity(UInt64 id)
	{
		//Search for entity in entity arr
		for (UInt64 i = 0; i < gLivingEntities; i++)
		{
			if (gEntitiesSignatures[i].id == id)
			{
				return &gEntitiesSignatures[i];
				break;
			}
		}
		EN_ASSERT(false, "Entity not found");
		return nullptr;
	}

	void removeComponentInternal(ComponentArray& componentArray, EntitySignature* entity)
	{
		ComponentHeader* header;
		Byte* offset;
		Byte* end;
		for (UInt32 i = 0; i < componentArray.count; i++)
		{
			offset = componentArray.data.get() + componentArray.size * i;
			header = (ComponentHeader*)offset;
			if (header->entity == entity->id) //Entity id matched, remove current i component
			{
				//End of arr, ignore it and decrease arr count by 1
				if (i == (componentArray.count - 1))
				{
					componentArray.count--;
					componentArray.entityToIndex.erase(entity->id);
					break;
				}
				else //Else copy end of arr to removed slot
				{
					end = componentArray.data.get() + componentArray.size * (componentArray.count - 1);
					memcpy(offset, end, componentArray.size);
					componentArray.count--;

					UInt64 entityOfRemovedIndex = ((ComponentHeader*)end)->entity;
					componentArray.entityToIndex[entityOfRemovedIndex] = i;
					break;
				}
			}
		}
	}

	void updateSystems(EntitySignature* entity)
	{
		for (auto& pair : gSystems)
		{
			System& system = pair.second;
			if ((system.signature & entity->signature) != system.signature) // If signature doesn't match, loop through all entities and remove them
			{
				system.entities.erase(entity->id);
			}
			else if ((system.signature & entity->signature) == system.signature)// Signature match, add to entity list
			{
				system.entities.insert(entity->id);
			}
		}
	}

	static void* constructComponent(UInt64 entity, ComponentType type, const void* extraData)
	{
		auto entitySignature = searchEntity(entity);

		//Update component arrays
		ComponentArray* pComponentArray;
		ComponentHeader* header;

		pComponentArray = &gComponentArrays[type];

		//Search array to check if this entity already has that component type
		for (UInt32 i = 0; i < pComponentArray->count; i++)
		{
			Byte* offset = pComponentArray->data.get() + pComponentArray->size * i;
			header = (ComponentHeader*)offset;
			if (header->entity == entity)
			{
				EN_WARN("Each entity can only have 1 type of component");
				return nullptr;
			}
		}
		ComponentHint componentData = getComponentHint(type);
		EN_ASSERT((pComponentArray->count * pComponentArray->size + componentData.size) < MAX_COMPONENT_ARRAY_BUFFER_SIZE, "Component array is full");
		//Copy data to end of array
		Byte* lastMemLoc = pComponentArray->data.get() + pComponentArray->count * pComponentArray->size;
		memcpy(lastMemLoc, extraData, pComponentArray->size);
		header = (ComponentHeader*)(lastMemLoc); //extract header
		header->entity = entity; // now this component has this entity as parent
		header->type = type;
		pComponentArray->count++;
		pComponentArray->entityToIndex.insert({ entity, pComponentArray->count - 1 });



		//Update entity's signature
		SET_BIT(entitySignature->signature, (UInt32)type);

		//Signal each systems
		updateSystems(entitySignature);

		return pComponentArray->data.get() + (pComponentArray->count - 1) * pComponentArray->size;
	}
}







