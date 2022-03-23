#include "pch.hpp"
#include "ECS.hpp"

#include "Lua.hpp"

#define SET_BIT(x, loc) x |= 1 << loc
#define UNSET_BIT(x, loc) x &= ~(1 << loc)

using namespace Core::ECS;
//Entity
static unsigned int gEntityCounter = 0;
static unsigned int gLivingEntities = 0;
static Arr<EntitySignature, MAX_ENTITIES> gEntitiesSignatures = {0};

//Component
static Map<ComponentType, ComponentArray> gComponentArrays;

//System
static Map<SystemType, System>  gSystems;

static EntitySignature& searchEntity(unsigned int id);
static void removeComponentInternal(ComponentArray& componentArray, EntitySignature& entity);
static void updateSystems(EntitySignature& entity);
static void constructComponent(unsigned int entity, ComponentType type, const void* extraData);

void Core::ECS::init()
{

	//Init components
	for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
	{
		ComponentData data = getComponentData((ComponentType)i);
		ComponentArray arr;
		arr.size = data.size;
		arr.count = 0;
		arr.data = new char[MAX_COMPONENT_ARRAY_BUFFER_SIZE];
		gComponentArrays.insert({ (ComponentType)i, arr });
	}

	//Renderer system
	unsigned int signature = 0;
	SET_BIT(signature, (unsigned int)ComponentType::TRANSFORM);
	SET_BIT(signature, (unsigned int)ComponentType::MODEL_RENDERER);
	gSystems[SystemType::RENDERER].signature = signature;

	signature = 0;
	SET_BIT(signature, (unsigned int)ComponentType::SCRIPT);
	gSystems[SystemType::SCRIPTING].signature = signature;

}

void Core::ECS::shutdown()
{
	

	for (const auto& pair : gComponentArrays)
	{
		delete[] pair.second.data;
	}
}

unsigned int Core::ECS::createEntity()
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
void Core::ECS::removeEntity(unsigned int entity)
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

const Arr<EntitySignature, MAX_ENTITIES>& Core::ECS::getEntitySignatures()
{
	return gEntitiesSignatures;
}

unsigned int Core::ECS::getEntityCount()
{
	return gLivingEntities;
}

void Core::ECS::addComponent(unsigned int entity, ComponentType type)
{
	ComponentData data = getComponentData(type);
	void* extraData = calloc(1, data.size);
	switch (type)
	{
	case ComponentType::TRANSFORM:
	{
		TransformComponent* transformComponent = (TransformComponent*)extraData;
		transformComponent->x = 0;
		transformComponent->y = 0;
		transformComponent->z = 0;
		transformComponent->rw = 1;
		transformComponent->rx = 0;
		transformComponent->ry = 0;
		transformComponent->rz = 0;
		transformComponent->sx = 1;
		transformComponent->sy = 1;
		transformComponent->sz = 1;
		break;
	}

	case ComponentType::SCRIPT:
	{
		ScriptComponent* component = (ScriptComponent*)extraData;
		component->L = Core::Lua::newScript(entity);
		break;
	}
	}

	constructComponent(entity, type, extraData);
	free(extraData);
}

void Core::ECS::removeComponent(unsigned int entity, ComponentType type)
{
	auto& entitySignature = searchEntity(entity);

	switch (type)
	{
	case ComponentType::SCRIPT:
	{
		ScriptComponent* component = (ScriptComponent*)ECS::getComponent(entity, ComponentType::SCRIPT);
		if(component)
			Lua::removeScript(component->L);
		break;
	}
	}

	//Update component arrays
	removeComponentInternal(gComponentArrays[type], entitySignature);

	//Update entity's signature
	UNSET_BIT(entitySignature.signature, (unsigned int)type);

	//Signal each systems
	updateSystems(entitySignature);
}

void* Core::ECS::getComponent(unsigned int entity, ComponentType type)
{
	auto& entitySignature = searchEntity(entity);

	ComponentArray* pComponentArray = &gComponentArrays[type];
	ComponentHeader* header;

	for (unsigned int i = 0; i < pComponentArray->count; i++)
	{
		char* offset = pComponentArray->data + pComponentArray->size * i;
		header = (ComponentHeader*)offset;
		if (header->entity == entitySignature.id)
		{
			return offset;
		}
	}

	return nullptr;

}

System& Core::ECS::getSystem(SystemType type)
{
	return gSystems[type];
}

unsigned int Core::ECS::getComponentArrayMemorySize(ComponentType type)
{
	ComponentArray* arr = &gComponentArrays[type];
	return arr->size * arr->count;
}

ComponentData Core::ECS::getComponentData(ComponentType type)
{
	Core::ECS::ComponentData result;
	switch (type)
	{
	case ComponentType::NAME:
		result.size = sizeof(NameComponent);
		strcpy(result.name, "NAME");
		break;
	case ComponentType::TRANSFORM:
		result.size = sizeof(TransformComponent);
		strcpy(result.name, "TRANSFORM");
		break;
	case ComponentType::MODEL_RENDERER:
		result.size = sizeof(ModelRendererComponent);
		strcpy(result.name, "MODEL_RENDERER");
		break;
	case ComponentType::SCRIPT:
		result.size = sizeof(ScriptComponent);
		strcpy(result.name, "SCRIPT");
		break;
	default:
		EN_ASSERT(false, "Unknown component: {}", (unsigned int)type);
	}

	return result;
}


EntitySignature& searchEntity(unsigned int id)
{
	using namespace Core::ECS;
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
	using namespace Core::ECS;
	ComponentHeader* header;
	char* offset;
	char* end;
	for (unsigned int i = 0; i < componentArray.count; i++)
	{
		offset = componentArray.data + componentArray.size * i;
		header = (ComponentHeader*)offset;
		if (header->entity == entity.id) //Entity id matched, remove current i component
		{
			//End of arr, ignore it and decrease arr count by 1
			if (i == (componentArray.count - 1))
			{
				componentArray.count--;
				break;
			}
			else //Else copy end of arr to removed slot
			{
				end = componentArray.data + componentArray.size * (componentArray.count - 1);
				memcpy(offset, end, componentArray.size);
				componentArray.count--;
				break;
			}
		}
	}
}

void updateSystems(EntitySignature& entity)
{
	using namespace Core::ECS;
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

static void constructComponent(unsigned int entity, ComponentType type, const void* extraData)
{
	using namespace Core::ECS;
	auto& entitySignature = searchEntity(entity);

	//Update component arrays
	ComponentArray* pComponentArray;
	ComponentHeader* header;

	pComponentArray = &gComponentArrays[type];

	//Search array to check if this entity already has that component type
	for (unsigned int i = 0; i < pComponentArray->count; i++)
	{
		char* offset = pComponentArray->data + pComponentArray->size * i;
		header = (ComponentHeader*)offset;
		if (header->entity == entity)
		{
			EN_WARN("Each entity can only have 1 component");
			return;
		}
	}
	ComponentData componentData = getComponentData(type);
	EN_ASSERT((pComponentArray->count * pComponentArray->size + componentData.size) < MAX_COMPONENT_ARRAY_BUFFER_SIZE, "Component array is full");
	//Copy data to end of array
	char* lastMemLoc = pComponentArray->data + pComponentArray->count * pComponentArray->size;
	memcpy(lastMemLoc, extraData, pComponentArray->size);
	header = (ComponentHeader*)(lastMemLoc); //extract header
	header->entity = entity; // now this component has this entity as parent
	pComponentArray->count++;

	//Update entity's signature
	SET_BIT(entitySignature.signature, (unsigned int)type);

	//Signal each systems
	updateSystems(entitySignature);
}


#undef SET_BIT
#undef UNSET_BIT


