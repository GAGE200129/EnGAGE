#pragma once


struct lua_State;

namespace Core
{
	struct Model;
}
namespace Core::ECS
{
	static constexpr unsigned int MAX_NAME_SIZE = 70u;
	static constexpr unsigned int MAX_COLLIDER_BUFFER_SIZE = 128u;
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
		char modelPath[MAX_NAME_SIZE];
		Model* pModel;
	};

	struct ScriptComponent
	{
		ComponentHeader header;
		char scriptPath[MAX_NAME_SIZE];
		lua_State* L;
	};

	struct RigidBodyComponent
	{
		ComponentHeader header;
		glm::vec3 velocity;
		glm::vec3 force;
		float mass;
		unsigned int colliderType;
		char colliderData[MAX_COLLIDER_BUFFER_SIZE];
	};

	struct ComponentData
	{
		const char* name;
		unsigned int size;
	};

	const ComponentData& getComponentData(ComponentType type);
	void initComponent(ComponentHeader* pHeader, ComponentType type);
	void destroyComponent(ComponentHeader* pHeader, ComponentType type);
}