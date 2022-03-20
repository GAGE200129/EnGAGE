#pragma once

#include "Components.hpp"

namespace Core
{
	namespace ECS
	{
		static constexpr unsigned int MAX_ENTITIES = 60;
		static constexpr unsigned int MAX_COMPONENTS = 32; //32 bits field
		static constexpr unsigned int MAX_COMPONENT_ARRAY_BUFFER_SIZE = 1048576u;

		enum class SystemType : unsigned int
		{
			RENDERER,
			COUNT
		};

		struct Entity
		{
			unsigned int id;
			unsigned int signature : MAX_COMPONENTS;
		};

		struct ComponentArray
		{
			unsigned int count;
			unsigned int size;
			char* data;
		};

		struct System
		{
			unsigned int signature : MAX_COMPONENTS; 
			unsigned int* entities;
			unsigned int entityCount;
		};

		void init();
		void shutdown();
		Entity createEntity();
		void removeEntity(Entity* pEntity);
		void constructComponent(Entity* pEntity, ComponentType type, const void* data);
		void* getEntityComponent(Entity* pEntity, ComponentType type);
	}
}