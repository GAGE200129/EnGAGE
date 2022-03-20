#include "pch.hpp"
#include "ECS.hpp"

#define SET_BIT(x, loc) x |= 1 << loc

namespace Core
{
	namespace ECS
	{

		//Entity
		static unsigned int gEntityCounter;
		static unsigned int gLivingEntities;

		//Component
		static ComponentArray* gComponentArrays;

		//System
		static System* gSystems;

		void init()
		{
			//Init entity
			gLivingEntities = 0;
			gEntityCounter = 0;

			//Init components
			gComponentArrays = (ComponentArray*)calloc((unsigned int)ComponentType::COUNT, sizeof(ComponentArray));
			for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
			{
				gComponentArrays[i].size = Core::sizeofComponent((ComponentType)i);
				gComponentArrays[i].count = 0;
				gComponentArrays[i].data = (char*)calloc(MAX_COMPONENT_ARRAY_BUFFER_SIZE, sizeof(char));
			}

			//Init systems
			gSystems = (System*)calloc((unsigned int)SystemType::COUNT, sizeof(System));
			for (unsigned int i = 0; i < (unsigned int)SystemType::COUNT; i++)
			{
				gSystems[i].signature = 0;;
				gSystems[i].entities = (unsigned int*)calloc(MAX_ENTITIES, sizeof(unsigned int));
				gSystems[i].entityCount = 0;
			}

			unsigned int signature = 0;
			SET_BIT(signature, (unsigned int)ComponentType::TRANSFORM);
			SET_BIT(signature, (unsigned int)ComponentType::MODEL_RENDERER);
			gSystems[(unsigned int)SystemType::RENDERER].signature = signature;

		}

		void shutdown()
		{
			for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
			{
				free(gComponentArrays[i].data);
			}

			free(gComponentArrays);

			for (unsigned int i = 0; i < (unsigned int)SystemType::COUNT; i++)
			{
				free(gSystems[i].entities);
			}

			free(gSystems);
		}

		Entity createEntity()
		{
			EN_ASSERT(gLivingEntities < MAX_ENTITIES, "MAX_ENTITIES = {}", MAX_ENTITIES);

			Entity entity;
			entity.id = gEntityCounter;
			entity.signature = 0;
			gEntityCounter++;
			gLivingEntities++;
			return entity;
		}
		void removeEntity(Entity* pEntity)
		{
			//Remove entity	
			gLivingEntities--;

			//Update component arrays
			for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
			{
				ComponentArray* arr = &gComponentArrays[i];
				ComponentHeader* header;
				char* offset;
				char* end;
				for (unsigned int i = 0; i < arr->count; i++)
				{
					offset = arr->data + arr->size * i;
					header = (ComponentHeader*)offset;
					if (header->entity == pEntity->id)
					{
						//End of arr, ignore it and decrease arr count by 1
						if (i == (arr->count - 1))
						{
							arr->count--;
							break;
						}
						else //Else copy end of arr to removed slot
						{
							end = arr->data + arr->size * (arr->count - 1);
							memcpy(offset, end, arr->size);
							arr->count--;
							break;
						}
					}
				}
			}


			//Signal each systems
			for (unsigned int i = 0; i < (unsigned int)SystemType::COUNT; i++)
			{
				System* system = &gSystems[i];
				if ((system->signature & pEntity->signature) == system->signature) // Check for signature
				{
					for (unsigned int j = 0; j < system->entityCount; j++) // Loop through every single entities in system
					{
						if (system->entities[j] == pEntity->id) // If there's an entity id match with entity about to be removed
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
			}
		}

		void constructComponent(Entity* pEntity, ComponentType type, const void* data)
		{
			//Update component arrays
			ComponentArray* pComponentArray;
			ComponentHeader* header;

			pComponentArray = &gComponentArrays[(unsigned int)type];
			EN_ASSERT((pComponentArray->count * pComponentArray->size + sizeofComponent(type)) < MAX_COMPONENT_ARRAY_BUFFER_SIZE,
				"Component array is full");
			char* lastMemLoc = pComponentArray->data + pComponentArray->count * pComponentArray->size;
			memcpy(lastMemLoc, data, pComponentArray->size);
			header = (Core::ComponentHeader*)(lastMemLoc); //extract header
			header->entity = pEntity->id;
			pComponentArray->count++;

			//Update entity's signature
			SET_BIT(pEntity->signature, (unsigned int)type);

			//Signal each systems
			for (unsigned int i = 0; i < (unsigned int)SystemType::COUNT; i++)
			{
				System* system = &gSystems[i];
				if ((system->signature & pEntity->signature) == system->signature) // Check for signature
				{
					system->entities[system->entityCount++] = pEntity->id;
				}
			}
		}

		void* getEntityComponent(Entity* pEntity, ComponentType type)
		{
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

	}
}

#undef SET_BIT