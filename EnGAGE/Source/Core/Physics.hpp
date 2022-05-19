#pragma once
#include "Messenger.hpp"
#include "Vertex.hpp"

class btRigidBody;
namespace Core::Physics
{
	//Colliders
	enum class CollisionShapeType : unsigned int
	{
		EMPTY,
		PLANE,
		SPHERE,
		BOX,
		COUNT
	};
	const char* getCollisionShapeName(CollisionShapeType type);

	void init();
	void clear();
	void shutdown();
	void onMessage(const Message* pMessage);
	void updateRigidBody(btRigidBody* rigidBody);
	btRigidBody* newRigidBody(unsigned int entityID);
	void update(float delta);
	void updateMap(const DynArr<Vertex>& vertices);
 
	void initCollisionShapeRuntime(btRigidBody* rigidBody, CollisionShapeType type, void* data = nullptr);

}