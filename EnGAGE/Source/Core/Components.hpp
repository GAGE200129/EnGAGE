#pragma once

struct lua_State;
class btRigidBody;
namespace Core
{
	struct Model;
}
namespace Core
{
	static constexpr unsigned int MAX_NAME_SIZE = 70u;
	//Component
	enum class ComponentType : unsigned int
	{
		NAME,
		TRANSFORM,
		MODEL_RENDERER,
		SCRIPT,
		RIGID_BODY,
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
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
		const Model* pModel;
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
		unsigned int collisionShapeType;
		btRigidBody* pRigidbody;
	};

	struct DirectionalLightComponent
	{
		ComponentHeader header;
		glm::vec3 direction;
		glm::vec3 color;
		float intensity;
	};

	struct PointLightComponent
	{
		ComponentHeader header;
		glm::vec3 color;
		float intensity;

		float constant;
		float linear;
		float exponent;
	};

	struct ComponentData
	{
		const char* name;
		unsigned int size;
	};


	const ComponentData& getComponentData(ComponentType type);
	
	void initComponent(unsigned int entityID, ComponentHeader* pHeader, ComponentType type);
	void destroyComponent(ComponentHeader* pHeader, ComponentType type);
}
