#include "pch.hpp"
#include "Physics.hpp"
#include "ECS.hpp"

#include "ECS.hpp"

class CustomMotionState : public btMotionState
{
	Core::TransformComponent* mGraphicsTransform;
public:
	CustomMotionState(Core::TransformComponent* graphicsTransform):
		mGraphicsTransform(graphicsTransform)
	{
	}

	inline void setGraphicsTransform(Core::TransformComponent* graphicsTransform)
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
	}

	void onMessage(const Message* pMessage)
	{
		switch(pMessage->type)
		{
		case MessageType::REMOVE_RIGID_BODY:
		{
			btRigidBody* body = nullptr;
			memcpy(&body, pMessage->message, sizeof(btRigidBody*));

			DynArr<btRigidBody*>::iterator removeIt;
			for (auto it = gRigidBodies.begin(); it != gRigidBodies.end(); it++)
			{
				if (*it == body)
				{
					removeIt = it;
					break;
				}
			}
			btRigidBody* removedBody = *removeIt;
			delete removedBody->getCollisionShape();
			delete removedBody->getMotionState();
			delete removedBody;

			gRigidBodies.erase(removeIt);

			break;
		}
		case MessageType::PHYSICS_UPDATE_RIGID_BODY:
		{
			btRigidBody* body = nullptr;
			memcpy(&body, pMessage->message, sizeof(btRigidBody*));
			updateRigidBody(body);
			break;
		}
		}

		if (auto initCollisionShape = Messenger::messageCast<MessageType::PHYSICS_INIT_COLLISION_SHAPE, PhysicsInitCollisionShapeMessage>(pMessage))
		{
			initCollisionShapeRuntime(initCollisionShape->body, (CollisionShapeType)initCollisionShape->type, (void*)initCollisionShape->arguments);
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
		TransformComponent* component = (TransformComponent*)ECS::getComponent(entityID, ComponentType::TRANSFORM);

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
}


