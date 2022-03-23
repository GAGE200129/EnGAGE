#pragma once

#include "Model.hpp"

struct lua_State;
namespace Core
{
	namespace ECS
	{
		static constexpr unsigned int MAX_ENTITIES = 3000;
		static constexpr unsigned int MAX_COMPONENTS = 32; //32 bits field
		static constexpr unsigned int MAX_COMPONENT_ARRAY_BUFFER_SIZE = 1048576u;
		static constexpr unsigned int MAX_NAME_SIZE = 50u;

		//Entity 
		struct EntitySignature
		{
			unsigned int id;
			unsigned int signature : MAX_COMPONENTS;
		};


		//Component
		struct ComponentData
		{
			char name[MAX_NAME_SIZE];
			unsigned int size;
		};

		struct ComponentHeader
		{
			unsigned int entity;
		};

		struct ComponentArray
		{
			unsigned int count;
			unsigned int size;
			char* data;
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
			lua_State* L;
		};



		//System
		enum class SystemType : unsigned int
		{
			RENDERER,
			SCRIPTING,
			COUNT
		};

		struct System
		{
			unsigned int signature : MAX_COMPONENTS; 
			Set<unsigned int> entities;
		};

		void init();
		void shutdown();
		unsigned int createEntity();
		void removeEntity(unsigned int entity);
		const Arr<EntitySignature, MAX_ENTITIES>& getEntitySignatures();
		unsigned int getEntityCount();
		void addComponent(unsigned int entity, ComponentType type);
		void removeComponent(unsigned int entity, ComponentType type);
		void* getComponent(unsigned int entity, ComponentType type);

		System& getSystem(SystemType type);

		unsigned int getComponentArrayMemorySize(ComponentType type);

		ComponentData getComponentData(ComponentType type);
	}
}