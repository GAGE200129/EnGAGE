#pragma once

#include "Components.hpp"


namespace Core
{
	
	namespace ECS
	{
		static constexpr unsigned int MAX_ENTITIES = 3000;
		static constexpr unsigned int MAX_COMPONENTS = 32; //32 bits field
		static constexpr unsigned int MAX_COMPONENT_ARRAY_BUFFER_SIZE = 1048576u;
		
		//Entity 
		struct EntitySignature
		{
			unsigned int id;
			unsigned int signature : MAX_COMPONENTS;
		};

		//System
		enum class SystemType : unsigned int
		{
			RENDERER,
			DIRECTIONAL_RENDERER,
			SCRIPTING,
			PHYSICS,
			COUNT
		};

		struct System
		{
			unsigned int signature : MAX_COMPONENTS; 
			Set<unsigned int> entities;
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