#pragma once
#include "Messenger.hpp"

class btRigidBody;
namespace Core::Physics
{
	//Colliders
	enum class CollisionShapeType : unsigned int
	{
		EMPTY,
		PLANE,
		SPHERE,
		COUNT
	};
	const char* getCollisionShapeName(CollisionShapeType type);

	void init();
	void shutdown();
	void onMessage(const Message* pMessage);
	void onRequest(Request* pRequest);
	void updateRigidBody(btRigidBody* rigidBody);
	btRigidBody* newRigidBody();
	void update(float delta);

	template<CollisionShapeType type>
	inline static void initCollisionShape(btRigidBody* rigidBody, void* data = nullptr)
	{
		static_assert(false);
	}

	template<>
	inline static void initCollisionShape<CollisionShapeType::SPHERE>(btRigidBody* rigidBody, void* data)
	{
		delete rigidBody->getCollisionShape();
		float* sphereData = (float*)data;

		float radius = data ? sphereData[0] : 1.0f;
		btSphereShape* shape = new btSphereShape(radius);
		rigidBody->setCollisionShape(shape);
		btVector3 inertia;
		shape->calculateLocalInertia(rigidBody->getMass(), inertia);
		rigidBody->setMassProps(rigidBody->getMass(), inertia);
		updateRigidBody(rigidBody);
	}

	template<>
	inline static void initCollisionShape<CollisionShapeType::PLANE>(btRigidBody* rigidBody, void* data)
	{
		delete rigidBody->getCollisionShape();
		float* planeData = (float*)data;
		btVector3 normal = {0, 1, 0};
		btScalar distance = 0.0f;

		if (data)
		{
			normal = btVector3(planeData[0], planeData[1], planeData[2]);
			distance = planeData[3];
		}
		btStaticPlaneShape* shape = new btStaticPlaneShape(normal, distance);
		rigidBody->setCollisionShape(shape);
		btVector3 inertia;
		shape->calculateLocalInertia(rigidBody->getMass(), inertia);
		rigidBody->setMassProps(rigidBody->getMass(), inertia);
		updateRigidBody(rigidBody);
	}

	inline static void initCollisionShapeRuntime(btRigidBody* rigidBody, CollisionShapeType type, void* data = nullptr)
	{
		switch (type)
		{
		case CollisionShapeType::PLANE:
			initCollisionShape<CollisionShapeType::PLANE>(rigidBody, data);
			return;
		case CollisionShapeType::SPHERE:
			initCollisionShape<CollisionShapeType::SPHERE>(rigidBody, data);
			return;
		}
	}

}