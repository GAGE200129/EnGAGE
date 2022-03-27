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
		static constexpr unsigned int MAX_COLLIDER_BUFFER_SIZE = 128u;

		//Entity 
		struct EntitySignature
		{
			unsigned int id;
			unsigned int signature : MAX_COMPONENTS;
		};

		//Physics component collider
		enum class ColliderType : unsigned int
		{
			NONE,
			SPHERE, // Float(radius)
			PLANE, // Vec3(Normal), Float(distance)
			COUNT
		};

		struct SphereCollider
		{
			float radius;
		};

		struct PlaneCollider
		{
			float x, y, z; // Normal
			float distance;
		};

		//Component
		enum class ComponentType : unsigned int
		{
			NAME,
			TRANSFORM,
			MODEL_RENDERER,
			SCRIPT,
			RIGID_BODY,
			COUNT
		};

		struct ComponentData
		{
			char name[MAX_NAME_SIZE];
			unsigned int size;
		};

		struct ComponentHeader
		{
			ComponentType type;
			unsigned int entity;
		};

		struct ComponentArray
		{
			unsigned int count;
			unsigned int size;
			Scope<char[]> data;
			Map<unsigned int, unsigned int> entityToIndex;
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

		struct RigidBodyComponent
		{
			ComponentHeader header;
			glm::vec3 velocity;
			glm::vec3 force;
			float mass;
			ColliderType colliderType;
			char colliderData[MAX_COLLIDER_BUFFER_SIZE];
		};
		

		//System
		enum class SystemType : unsigned int
		{
			RENDERER,
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

		ComponentData getComponentData(ComponentType type);
	}
}