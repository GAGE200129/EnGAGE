#pragma once
#include "Core/Messenger/Messenger.hpp"
#include "Core/Data/Triangle.hpp"

class btRigidBody;
namespace Core::Physics
{
	//Colliders
	enum class CollisionShapeType : unsigned int
	{
		EMPTY,
		SPHERE,
		CAPSULE,
		BOX,
		COUNT
	};
	const char* getCollisionShapeName(CollisionShapeType type);
	const Vec3& getGravity();

	void init();
	void clear();
	void shutdown();
	void updateRigidBody(btRigidBody* rigidBody);
	void removeRigidBody(btRigidBody* rigidBody);
	btRigidBody* newRigidBody(unsigned int entityID);
	void update(float delta);
	void updateMap(const DynArr<Triangle>& vertices);

	
	void initColShapeSphere(btRigidBody* rigidBody, F32 radius);
	void initColShapePlane(btRigidBody* rigidBody, F32 x, F32 y, F32 z, F32 distance);
	void initColShapeBox(btRigidBody* rigidBody, F32 x, F32 y, F32 z);
	void initColShapeCapsule(btRigidBody* rigidBody, F32 radius, F32 height);
	void initCollisionShapeRuntime(btRigidBody* rigidBody, CollisionShapeType type);

}