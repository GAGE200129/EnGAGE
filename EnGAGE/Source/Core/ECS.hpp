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
			UInt64 id;
			UInt32 signature : MAX_COMPONENTS;
		};


		void init();
		void shutdown();
		void updateRemovedEntities();
		UInt64 createEntity();
		void removeEntity(UInt64 entity);
		void markForRemove(UInt64 entity);
		const Arr<EntitySignature, MAX_ENTITIES>& getEntitySignatures();
		UInt64 getEntityCount();
		void* addComponent(UInt64 entity, ComponentType type);
		void removeComponent(UInt64 entity, ComponentType type);
		void* getComponent(UInt64 entity, ComponentType type);

		System& getSystem(SystemType type);

		UInt64 getComponentArrayMemorySize(ComponentType type);
		
		
	}
}