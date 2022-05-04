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
		BOX,
		COUNT
	};
	const char* getCollisionShapeName(CollisionShapeType type);

	void init();
	void clearAllRigidBodies();
	void shutdown();
	void onMessage(const Message* pMessage);
	void updateRigidBody(btRigidBody* rigidBody);
	btRigidBody* newRigidBody(unsigned int entityID);
	void update(float delta);

	inline static void recalculateIntertia(btCollisionShape* pShape, btRigidBody* pBody)
	{
		btVector3 inertia;
		pShape->calculateLocalInertia(pBody->getMass(), inertia);
		pBody->setMassProps(pBody->getMass(), inertia);
		updateRigidBody(pBody);
	}

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
		recalculateIntertia(shape, rigidBody);
	}

	template<>
	inline static void initCollisionShape<CollisionShapeType::PLANE>(btRigidBody* rigidBody, void* data)
	{
		delete rigidBody->getCollisionShape();
		btVector3 normal = {0, 1, 0};
		btScalar distance = 0.0f;

		float* planeData = (float*)data;
		if (data)
		{
			normal = btVector3(planeData[0], planeData[1], planeData[2]);
			distance = planeData[3];
		}
		btStaticPlaneShape* shape = new btStaticPlaneShape(normal, distance);
		rigidBody->setCollisionShape(shape);
		recalculateIntertia(shape, rigidBody);
	}

	template<>
	inline static void initCollisionShape<CollisionShapeType::BOX>(btRigidBody* rigidBody, void* data)
	{
		delete rigidBody->getCollisionShape();
		btVector3 halfExtends = {1.0f, 1.0f, 1.0f};

		float* boxData = (float*)data;
		if (data)
		{
			halfExtends.setX(boxData[0]);
			halfExtends.setY(boxData[1]);
			halfExtends.setZ(boxData[2]);
		}

		btBoxShape* shape = new btBoxShape(halfExtends);
		rigidBody->setCollisionShape(shape);
		recalculateIntertia(shape, rigidBody);
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
		case CollisionShapeType::BOX:
			initCollisionShape<CollisionShapeType::BOX>(rigidBody, data);
			return;
		}
	}

}