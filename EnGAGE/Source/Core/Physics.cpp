#include "pch.hpp"
#include "Physics.hpp"
#include "ECS.hpp"

#include "ECS.hpp"
#include "Components/Transform.hpp"

class CustomMotionState : public btMotionState
{
	Core::Transform::Component* mGraphicsTransform;
public:
	CustomMotionState(Core::Transform::Component* graphicsTransform):
		mGraphicsTransform(graphicsTransform)
	{
	}

	inline void setGraphicsTransform(Core::Transform::Component* graphicsTransform)
	{
		mGraphicsTransform = graphicsTransform;
	}

	///synchronizes world transform from user to physics
	virtual void getWorldTransform(btTransform& centerOfMassWorldTrans) const override
	{
		btTransform transform;
		transform.setIdentity();
		if (mGraphicsTransform != nullptr)
		{
			transform.setOrigin(btVector3(mGraphicsTransform->x, mGraphicsTransform->y, mGraphicsTransform->z));
			transform.setRotation(btQuaternion(mGraphicsTransform->rx, mGraphicsTransform->ry, mGraphicsTransform->rz, mGraphicsTransform->rw));
		}
		centerOfMassWorldTrans = transform;
	}

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans) override
	{
		if (mGraphicsTransform != nullptr)
		{
			mGraphicsTransform->x = centerOfMassWorldTrans.getOrigin().x();
			mGraphicsTransform->y = centerOfMassWorldTrans.getOrigin().y();
			mGraphicsTransform->z = centerOfMassWorldTrans.getOrigin().z();

			mGraphicsTransform->rw = centerOfMassWorldTrans.getRotation().w();
			mGraphicsTransform->rx = centerOfMassWorldTrans.getRotation().x();
			mGraphicsTransform->ry = centerOfMassWorldTrans.getRotation().y();
			mGraphicsTransform->rz = centerOfMassWorldTrans.getRotation().z();
		}
	}
};

namespace Core::Physics
{
	static glm::vec3 gGravity = { 0, -9.8f, 0 };
	static btDispatcher* gDispatcher;
	static btCollisionConfiguration* gConfig;
	static btBroadphaseInterface* gBroadphase;
	static btConstraintSolver* gSolver;
	static btDynamicsWorld* gWorld;
	static DynArr<btRigidBody*> gRigidBodies;

	static void recalculateIntertia(btCollisionShape* pShape, btRigidBody* pBody)
	{
		btVector3 inertia;
		pShape->calculateLocalInertia(pBody->getMass(), inertia);
		pBody->setMassProps(pBody->getMass(), inertia);
		updateRigidBody(pBody);
	}

	template<CollisionShapeType type>
	static void initCollisionShape(btRigidBody* rigidBody, void* data = nullptr)
	{
		static_assert(false);
	}

	template<>
	static void initCollisionShape<CollisionShapeType::SPHERE>(btRigidBody* rigidBody, void* data)
	{
		delete rigidBody->getCollisionShape();
		float* sphereData = (float*)data;

		float radius = data ? sphereData[0] : 1.0f;
		btSphereShape* shape = new btSphereShape(radius);
		rigidBody->setCollisionShape(shape);
		recalculateIntertia(shape, rigidBody);
	}

	template<>
	static void initCollisionShape<CollisionShapeType::PLANE>(btRigidBody* rigidBody, void* data)
	{
		delete rigidBody->getCollisionShape();
		btVector3 normal = { 0, 1, 0 };
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
	static void initCollisionShape<CollisionShapeType::BOX>(btRigidBody* rigidBody, void* data)
	{
		delete rigidBody->getCollisionShape();
		btVector3 halfExtends = { 1.0f, 1.0f, 1.0f };

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

	
	const char* getCollisionShapeName(CollisionShapeType type)
	{
		switch (type)
		{
		case CollisionShapeType::EMPTY:
			return "EMPTY";
		case CollisionShapeType::PLANE:
			return "PLANE";
		case CollisionShapeType::SPHERE:
			return "SPHERE";
		case CollisionShapeType::BOX:
			return "BOX";
		}
		EN_ASSERT(false, "Unknown collision shape: {}", (unsigned int)type);
		return "";
	}

	void init()
	{
		gConfig = new btDefaultCollisionConfiguration();
		gDispatcher = new btCollisionDispatcher(gConfig);
		gBroadphase = new btDbvtBroadphase();
		gSolver = new btSequentialImpulseConstraintSolver();
		gWorld = new btDiscreteDynamicsWorld(gDispatcher, gBroadphase, gSolver, gConfig);
		gWorld->setGravity(btVector3(gGravity.x, gGravity.y, gGravity.z));

	}

	void clearAllRigidBodies()
	{
		for (auto& body : gRigidBodies)
		{
			gWorld->removeRigidBody(body);
			delete body->getCollisionShape();
			delete body->getMotionState();
			delete body;
		}
		gRigidBodies.clear();
	}

	void shutdown()
	{
		clearAllRigidBodies();
		delete gConfig;
		delete gDispatcher;
		delete gBroadphase;
		delete gSolver;
		delete gWorld;
	}

	void onMessage(const Message* pMessage)
	{
		if (auto message = Messenger::messageCast<MessageType::REMOVE_RIGID_BODY, RemoveRigidBodyMessage>(pMessage))
		{
			btRigidBody* body = message->body;

			DynArr<btRigidBody*>::iterator removeIt = gRigidBodies.end();
			for (auto it = gRigidBodies.begin(); it != gRigidBodies.end(); it++)
			{
				if (*it == body)
				{
					removeIt = it;
					break;
				}
			}

			EN_ASSERT(removeIt != gRigidBodies.end(), "Rigidbody not found !");

			btRigidBody* removedBody = *removeIt;
			delete removedBody->getCollisionShape();
			delete removedBody->getMotionState();
			delete removedBody;

			gRigidBodies.erase(removeIt);
		}
		else if (auto message = Messenger::messageCast<MessageType::PHYSICS_UPDATE_RIGID_BODY, PhysicsUpdateRigidBodyMessage>(pMessage))
		{
			updateRigidBody(message->body);
		} else if (auto message = Messenger::messageCast<MessageType::PHYSICS_INIT_COLLISION_SHAPE, PhysicsInitCollisionShapeMessage>(pMessage))
		{
			initCollisionShapeRuntime(message->body, (CollisionShapeType)message->type, (void*)message->arguments);
		}
	}


	void updateRigidBody(btRigidBody* rigidBody)
	{
		gWorld->removeRigidBody(rigidBody);
		rigidBody->activate();
		gWorld->addRigidBody(rigidBody);
	}

	btRigidBody* newRigidBody(unsigned int entityID)
	{
		Transform::Component* component = (Transform::Component*)ECS::getComponent(entityID, ComponentType::TRANSFORM);

		btMotionState* pMotionState = new CustomMotionState(component);
		btEmptyShape* pShape = new btEmptyShape();

		btRigidBody::btRigidBodyConstructionInfo info(0, pMotionState, pShape);
		btRigidBody* pRigidBody = new btRigidBody(info);
		pRigidBody->setRestitution(0.9f);
		pRigidBody->setDamping(0.2f, 0.2f);

		gRigidBodies.push_back(pRigidBody);
		gWorld->addRigidBody(pRigidBody);

		return pRigidBody;
	}

	void update(float delta)
	{
		gWorld->stepSimulation(delta);		
	}
	void initCollisionShapeRuntime(btRigidBody* rigidBody, CollisionShapeType type, void* data)
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


