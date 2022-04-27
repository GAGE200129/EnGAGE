#include "pch.hpp"
#include "Physics.hpp"

#include "ECS.hpp"



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
		}
		EN_ASSERT(false, "Unknown collision shape: {}", (unsigned int)type);
		return "";
	}

	

	void Physics::init()
	{
		gConfig = new btDefaultCollisionConfiguration();
		gDispatcher = new btCollisionDispatcher(gConfig);
		gBroadphase = new btDbvtBroadphase();
		gSolver = new btSequentialImpulseConstraintSolver();
		gWorld = new btDiscreteDynamicsWorld(gDispatcher, gBroadphase, gSolver, gConfig);
		gWorld->setGravity(btVector3(gGravity.x, gGravity.y, gGravity.z));

	}

	void Physics::shutdown()
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
		case MessageType::PHYSICS_UPDATE_RIGID_BODY:
		{
			btRigidBody* body = nullptr;
			memcpy(&body, pMessage->message, sizeof(btRigidBody*));
			updateRigidBody(body);
			break;
		}
		case MessageType::INIT_COLLISION_SHAPE:
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

	void onRequest(Request* pRequest)
	{
		switch (pRequest->type)
		{
		case RequestType::NEW_RIGID_BODY:
		{
			btRigidBody* body = newRigidBody();
			memcpy(pRequest->data, &body, sizeof(btRigidBody*));
			break;
		}
		}
	}

	void updateRigidBody(btRigidBody* rigidBody)
	{
		gWorld->removeRigidBody(rigidBody);
		gWorld->addRigidBody(rigidBody);
	}

	btRigidBody* newRigidBody()
	{

		btMotionState* pMotionState = new btDefaultMotionState();
		btEmptyShape* pShape = new btEmptyShape();

		btRigidBody::btRigidBodyConstructionInfo info(0, pMotionState, pShape);
		btRigidBody* pRigidBody = new btRigidBody(info);

		gRigidBodies.push_back(pRigidBody);
		gWorld->addRigidBody(pRigidBody);

		return pRigidBody;
	}

	void Physics::update(float delta)
	{
		gWorld->stepSimulation(delta);

		using namespace ECS;
		const System& system = getSystem(SystemType::PHYSICS);
		const auto& entities = system.entities;
		for (auto e : entities)
		{
			RigidBodyComponent* pRigidBody = (RigidBodyComponent*)getComponent(e, ComponentType::RIGID_BODY);
			TransformComponent* pTransform = (TransformComponent*)getComponent(e, ComponentType::TRANSFORM);
			pRigidBody->pRigidbody->activate();
			const btTransform& t = pRigidBody->pRigidbody->getCenterOfMassTransform();

			const btVector3& p = t.getOrigin();
			const btQuaternion& r = t.getRotation();

			pTransform->isStatic = false;
			pTransform->x = p.x();
			pTransform->y = p.y();
			pTransform->z = p.z();

			pTransform->rw = r.w();
			pTransform->rx = r.x();
			pTransform->ry = r.y();
			pTransform->rz = r.z();

		}
		
	}
}


