#pragma once

#include "Model.hpp"

namespace Core
{
	namespace ECS
	{
		static constexpr unsigned int MAX_ENTITIES = 3000;
		static constexpr unsigned int MAX_COMPONENTS = 32; //32 bits field
		static constexpr unsigned int MAX_COMPONENT_ARRAY_BUFFER_SIZE = 1048576u;
		static constexpr unsigned int MAX_NAME_SIZE = 50u;

		struct ComponentHeader
		{
			unsigned int entity;
		};

		enum class ComponentType : unsigned int
		{
			NAME,
			TRANSFORM,
			MODEL_RENDERER,
			SCRIPT,
			COUNT
		};

		struct NameComponent
		{
			ComponentHeader header;
			char name[MAX_NAME_SIZE];
		};

		struct TransformComponent
		{
			ComponentHeader header;
			float x, y, z,
				rw, rx, ry, rz,
				sx, sy, sz;
		};

		struct ModelRendererComponent
		{
			ComponentHeader header;
			Model* pModel;
		};

		struct ScriptComponent
		{
			ComponentHeader header;
			void (*init)();
			void (*update)(float delta);
		};

	
		enum class SystemType : unsigned int
		{
			RENDERER,
			SCRIPTING,
			COUNT
		};

		struct EntitySignature
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
			unsigned int entities[MAX_ENTITIES];
			unsigned int entityCount;
		};

		void init();
		void shutdown();
		unsigned int createEntity();
		void removeEntity(unsigned int entity);
		EntitySignature* getEntitySignatures();
		unsigned int getEntityCount();
		void constructComponent(unsigned int entity, ComponentType type, const void* data = nullptr);
		void removeComponent(unsigned int entity, ComponentType type);
		void* getComponent(unsigned int entity, ComponentType type);

		System* getSystem(SystemType type);

		unsigned int getComponentArrayMemorySize(ComponentType type);
		const char* getComponentArrayName(ComponentType type);
		unsigned int sizeofComponent(ComponentType type);
	}
}