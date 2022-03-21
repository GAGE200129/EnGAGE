#include "pch.hpp"
#include "ECS.hpp"

#define SET_BIT(x, loc) x |= 1 << loc
#define UNSET_BIT(x, loc) x &= ~(1 << loc)

//Entity
static unsigned int gEntityCounter;
static unsigned int gLivingEntities;
static Core::ECS::EntitySignature gEntitiesSignatures[Core::ECS::MAX_ENTITIES];

//Component
static Core::ECS::ComponentArray gComponentArrays[(unsigned int)Core::ECS::ComponentType::COUNT];

//System
static Core::ECS::System gSystems[(unsigned int)Core::ECS::SystemType::COUNT];

static Core::ECS::EntitySignature* searchEntity(unsigned int id);
static void removeComponentInternal(Core::ECS::ComponentArray* pComponentArray, Core::ECS::EntitySignature* pEntity);
static void updateSystems(Core::ECS::System* pSystems, Core::ECS::EntitySignature* pEntity);

void Core::ECS::init()
{
	//Init entity
	gLivingEntities = 0;
	gEntityCounter = 0;
	memset(gEntitiesSignatures, 0, sizeof(gEntitiesSignatures));
	

	//Init components
	for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
	{
		gComponentArrays[i].size = sizeofComponent((ComponentType)i);
		gComponentArrays[i].count = 0;
		gComponentArrays[i].data = (char*)calloc(MAX_COMPONENT_ARRAY_BUFFER_SIZE, sizeof(char));
	}

	//Init systems
	for (unsigned int i = 0; i < (unsigned int)SystemType::COUNT; i++)
	{
		gSystems[i].signature = 0;;
		memset(gSystems[i].entities, 0, sizeof(unsigned int) * MAX_ENTITIES);
		gSystems[i].entityCount = 0;
	}

	unsigned int signature = 0;
	SET_BIT(signature, (unsigned int)ComponentType::TRANSFORM);
	SET_BIT(signature, (unsigned int)ComponentType::MODEL_RENDERER);
	gSystems[(unsigned int)SystemType::RENDERER].signature = signature;

}

void Core::ECS::shutdown()
{
	for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
	{
		free(gComponentArrays[i].data);
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
	EntitySignature* pEntity = searchEntity(entity);

	//Update component arrays
	for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
	{
		removeComponentInternal(&gComponentArrays[i], pEntity);
	}
	//Clear signature
	pEntity->signature = 0;

	//Signal each systems
	updateSystems(gSystems, pEntity);

	//Remove entity	
	for (unsigned int i = 0; i < gLivingEntities; i++)
	{
		if (gEntitiesSignatures[i].id == pEntity->id)
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

Core::ECS::EntitySignature* Core::ECS::getEntitySignatures()
{
	return gEntitiesSignatures;
}

unsigned int Core::ECS::getEntityCount()
{
	return gLivingEntities;
}

void Core::ECS::constructComponent(unsigned int entity, ComponentType type, const void* data)
{
	EntitySignature* pEntity = searchEntity(entity);

	//Update component arrays
	ComponentArray* pComponentArray;
	ComponentHeader* header;

	pComponentArray = &gComponentArrays[(unsigned int)type];

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
	EN_ASSERT((pComponentArray->count * pComponentArray->size + sizeofComponent(type)) < MAX_COMPONENT_ARRAY_BUFFER_SIZE, "Component array is full");
	//Copy data to end of array
	char* lastMemLoc = pComponentArray->data + pComponentArray->count * pComponentArray->size;
	if (data)
		memcpy(lastMemLoc, data, pComponentArray->size);
	else
		memset(lastMemLoc, 0, pComponentArray->size);
	header = (ComponentHeader*)(lastMemLoc); //extract header
	header->entity = entity; // now this component has this entity as parent
	pComponentArray->count++;

	//Update entity's signature
	SET_BIT(pEntity->signature, (unsigned int)type);

	//Signal each systems
	updateSystems(gSystems, pEntity);
}

void Core::ECS::removeComponent(unsigned int entity, ComponentType type)
{
	EntitySignature* pEntity = searchEntity(entity);
	//Update component arrays
	removeComponentInternal(&gComponentArrays[(unsigned int)type], pEntity);

	//Update entity's signature
	UNSET_BIT(pEntity->signature, (unsigned int)type);

	//Signal each systems
	updateSystems(gSystems, pEntity);
}

void* Core::ECS::getComponent(unsigned int e, ComponentType type)
{
	EntitySignature* pEntity = searchEntity(e);

	ComponentArray* pComponentArray = &gComponentArrays[(unsigned int)type];
	ComponentHeader* header;

	for (unsigned int i = 0; i < pComponentArray->count; i++)
	{
		char* offset = pComponentArray->data + pComponentArray->size * i;
		header = (ComponentHeader*)offset;
		if (header->entity == pEntity->id)
		{
			return offset;
		}
	}

	return nullptr;

}

Core::ECS::System* Core::ECS::getSystem(SystemType type)
{
	return &gSystems[(unsigned int)type];
}

unsigned int Core::ECS::getComponentArrayMemorySize(ComponentType type)
{
	ComponentArray* arr = &gComponentArrays[(unsigned int)type];
	return arr->size * arr->count;
}

const char* Core::ECS::getComponentArrayName(ComponentType type)
{
	switch (type)
	{
	case ComponentType::NAME:
		return "Name";
	case ComponentType::TRANSFORM:
		return "Transform";
	case ComponentType::MODEL_RENDERER:
		return "ModelRenderer";
	default:
		EN_ASSERT(false, "Unknown component: {}", (unsigned int)type);
		return "";
	}
}

unsigned int Core::ECS::sizeofComponent(ComponentType type)
{
	switch (type)
	{
	case ComponentType::NAME:
		return sizeof(NameComponent);
	case ComponentType::TRANSFORM:
		return sizeof(TransformComponent);
	case ComponentType::MODEL_RENDERER:
		return sizeof(ModelRendererComponent);
	default:
		EN_ASSERT(false, "Unknown component: {}", (unsigned int)type);
		return 0;
	}
}

Core::ECS::EntitySignature* searchEntity(unsigned int id)
{
	using namespace Core::ECS;
	//Search for entity in entity arr
	EntitySignature* pEntity = nullptr;
	for (unsigned int i = 0; i < gLivingEntities; i++)
	{
		if (gEntitiesSignatures[i].id == id)
		{
			pEntity = &gEntitiesSignatures[i];
			break;
		}
	}
	EN_ASSERT(pEntity != nullptr,
		"Entity not found");
	return pEntity;
}

void removeComponentInternal(Core::ECS::ComponentArray* pComponentArray, Core::ECS::EntitySignature* pEntity)
{
	using namespace Core::ECS;
	ComponentHeader* header;
	char* offset;
	char* end;
	for (unsigned int i = 0; i < pComponentArray->count; i++)
	{
		offset = pComponentArray->data + pComponentArray->size * i;
		header = (ComponentHeader*)offset;
		if (header->entity == pEntity->id) //Entity id matched, remove current i component
		{
			//End of arr, ignore it and decrease arr count by 1
			if (i == (pComponentArray->count - 1))
			{
				pComponentArray->count--;
				break;
			}
			else //Else copy end of arr to removed slot
			{
				end = pComponentArray->data + pComponentArray->size * (pComponentArray->count - 1);
				memcpy(offset, end, pComponentArray->size);
				pComponentArray->count--;
				break;
			}
		}
	}
}

void updateSystems(Core::ECS::System* pSystems, Core::ECS::EntitySignature* pEntity)
{
	using namespace Core::ECS;
	for (unsigned int i = 0; i < (unsigned int)SystemType::COUNT; i++)
	{
		System* system = &gSystems[i];
		if ((system->signature & pEntity->signature) != system->signature) // If signature doesn't match, loop through all entities and remove them
		{
			for (unsigned int j = 0; j < system->entityCount; j++) 
			{
				if (system->entities[j] == pEntity->id) // If there's an entity id match with this entity 
				{

					if (j == (system->entityCount - 1)) { // End of arr, decrease count by 1
						system->entityCount--;
						break;
					}
					else // Move entity at end of arr to removed loc and decrease count by 1
					{
						unsigned int end = system->entities[system->entityCount - 1];
						system->entities[j] = end;
						system->entityCount--;
						break;
					}
				}
			}
		}
		else if((system->signature & pEntity->signature) == system->signature)// Signature match, add to entity list
		{
			//Prevent duplications
			bool found = false;
			for (unsigned int j = 0; j < system->entityCount; j++)
			{
				if (system->entities[j] == pEntity->id)
				{
					found = true;
				}
			}
			if(!found)
				system->entities[system->entityCount++] = pEntity->id;
		}
	}
}


#undef SET_BIT
#undef UNSET_BIT


