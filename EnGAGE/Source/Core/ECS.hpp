#pragma once

#include "Components.hpp"
#include "Systems.hpp"
#include "ECSConstants.hpp"
#include "Messenger.hpp"

namespace Core
{
	
	namespace ECS
	{
			
		//Entity 
		struct EntitySignature
		{
			unsigned int id;
			unsigned int signature : MAX_COMPONENTS;
		};


		void init();
		void shutdown();
		void updateRemovedEntities();
		unsigned int createEntity();
		void removeEntity(unsigned int entity);
		void markForRemove(unsigned int entity);
		const Arr<EntitySignature, MAX_ENTITIES>& getEntitySignatures();
		unsigned int getEntityCount();
		void* addComponent(unsigned int entity, ComponentType type);
		void removeComponent(unsigned int entity, ComponentType type);
		void* getComponent(unsigned int entity, ComponentType type);

		System& getSystem(SystemType type);

		unsigned int getComponentArrayMemorySize(ComponentType type);
		
		
	}
}