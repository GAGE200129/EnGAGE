#pragma once
#include "Core/Messenger/Messenger.hpp"
#include "Triangle.hpp"

class btRigidBody;
namespace Core::Physics
{
	//Colliders
	enum class CollisionShapeType : unsigned int
	{
		EMPTY,
		SPHERE,
		BOX,
		COUNT
	};
	const char* getCollisionShapeName(CollisionShapeType type);

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
	void initCollisionShapeRuntime(btRigidBody* rigidBody, CollisionShapeType type);

}