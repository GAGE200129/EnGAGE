#pragma once

struct lua_State;
class btRigidBody;
namespace Core
{
	struct Model;
}
namespace Core
{
	static constexpr UInt8 MAX_NAME_SIZE = 70u;
	//Component
	enum class ComponentType : UInt8
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
		UInt64 entity;
	};

	struct ComponentArray
	{
		UInt64 count;
		UInt64 size;
		Scope<Byte[]> data;
		Map<UInt64, UInt64> entityToIndex;
	};


	struct NameComponent
	{
		ComponentHeader header;
		Byte name[MAX_NAME_SIZE];
	};

	struct TransformComponent
	{
		ComponentHeader header;
		F32 x, y, z,
			rw, rx, ry, rz,
			sx, sy, sz;
	};

	struct ModelRendererComponent
	{
		ComponentHeader header;
		Byte modelPath[MAX_NAME_SIZE];
		const Model* pModel;
	};

	struct ScriptComponent
	{
		ComponentHeader header;
		Byte scriptPath[MAX_NAME_SIZE];
		lua_State* L;
	};

	struct RigidBodyComponent
	{
		ComponentHeader header;
		UInt8 collisionShapeType;
		btRigidBody* pRigidbody;
	};

	struct DirectionalLightComponent
	{
		ComponentHeader header;
		Vec3 direction;
		Vec3 color;
		F32 intensity;
	};

	struct PointLightComponent
	{
		ComponentHeader header;
		Vec3 color;
		F32 intensity;

		F32 constant;
		F32 linear;
		F32 exponent;
	};

	struct ComponentData
	{
		CStr name;
		UInt32 size;
	};


	const ComponentData& getComponentData(ComponentType type);
	
	void initComponent(UInt64 entityID, ComponentHeader* pHeader, ComponentType type);
	void destroyComponent(ComponentHeader* pHeader, ComponentType type);
}
