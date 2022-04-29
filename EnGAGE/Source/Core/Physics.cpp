#include "pch.hpp"
#include "Physics.hpp"

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

	void shutdown()
	{
		for (auto& body : gRigidBodies)
		{
			delete body->getCollisionShape();
			delete body->getMotionState();
			delete body;
		}

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
		case MessageType::PHYSICS_INIT_COLLISION_SHAPE:
		{
			struct Data
			{
				btRigidBody* body;
				unsigned int type;
				unsigned char arguments[50];
			} data;

			memcpy(&data, pMessage->message, sizeof(Data));
			
			initCollisionShapeRuntime(data.body, (CollisionShapeType)data.type, data.arguments);

			break;
		}
		}
	}

	bool onRequest(Request* pRequest)
	{
		switch (pRequest->type)
		{
		case RequestType::NEW_RIGID_BODY:
		{
			unsigned int entityID;
			memcpy(&entityID, pRequest->data, sizeof(unsigned int));
			btRigidBody* body = newRigidBody(entityID);
			memcpy(pRequest->data, &body, sizeof(btRigidBody*));
			return true;
		}
		}

		return false;
	}

	void updateRigidBody(btRigidBody* rigidBody)
	{
		rigidBody->activate();
		gWorld->removeRigidBody(rigidBody);
		gWorld->addRigidBody(rigidBody);
	}

	btRigidBody* newRigidBody(unsigned int entityID)
	{
		struct Data
		{
			unsigned int entityID;
			ComponentType type;
		} data;
		data.entityID = entityID;
		data.type = ComponentType::TRANSFORM;

		auto request = Messenger::request(RequestType::ENTITY_COMPONENT, sizeof(Data), &data);

		TransformComponent* component = nullptr;

		memcpy(&component, request.data, sizeof(TransformComponent*));

		btMotionState* pMotionState = new CustomMotionState(component);
		btEmptyShape* pShape = new btEmptyShape();

		btRigidBody::btRigidBodyConstructionInfo info(0, pMotionState, pShape);
		btRigidBody* pRigidBody = new btRigidBody(info);
		pRigidBody->setRestitution(0.9f);

		gRigidBodies.push_back(pRigidBody);
		gWorld->addRigidBody(pRigidBody);

		return pRigidBody;
	}

	void update(float delta)
	{
		gWorld->stepSimulation(delta);		
	}
}


