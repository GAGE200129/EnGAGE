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
	static glm::vec3 gGravity = { 0, 0, 0 };
	static btDispatcher* gDispatcher;
	static btCollisionConfiguration* gConfig;
	static btBroadphaseInterface* gBroadphase;
	static btConstraintSolver* gSolver;
	static btDynamicsWorld* gWorld;
	static DynArr<btRigidBody*> gRigidBodies;
	static btCollisionObject* gMapObject;
	static btTriangleMesh* gMapMesh;

	static void recalculateIntertia(btCollisionShape* pShape, btRigidBody* pBody)
	{
		btVector3 inertia;
		pShape->calculateLocalInertia(pBody->getMass(), inertia);
		pBody->setMassProps(pBody->getMass(), inertia);
		updateRigidBody(pBody);
	}

	
	const char* getCollisionShapeName(CollisionShapeType type)
	{
		switch (type)
		{
		case CollisionShapeType::EMPTY:
			return "EMPTY";
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

		gMapObject = new btCollisionObject();
		gMapObject->setCollisionShape(new btEmptyShape());
		gWorld->addCollisionObject(gMapObject);
		gMapMesh = nullptr;
	}


	void clear()
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
		clear();
		delete gWorld;
		delete gConfig;
		delete gDispatcher;
		delete gSolver;
		delete gBroadphase;
	
		if (gMapMesh)
			delete gMapMesh;
		delete gMapObject->getCollisionShape();
		delete gMapObject;
	}


	void updateRigidBody(btRigidBody* rigidBody)
	{
		gWorld->removeRigidBody(rigidBody);
		rigidBody->activate();
		gWorld->addRigidBody(rigidBody);
	}

	void removeRigidBody(btRigidBody* body)
	{
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
		gWorld->removeRigidBody(removedBody);

		gRigidBodies.erase(removeIt);
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
	void updateMap(const DynArr<Vertex>& vertices)
	{
		EN_ASSERT(vertices.size() % 3 == 0, "Missing vertex");

		if (vertices.size() == 0)
		{
			gWorld->removeCollisionObject(gMapObject);
			return;
		}

		//Delete old stuffs
		if (gMapMesh) delete gMapMesh;
		delete gMapObject->getCollisionShape();
		gWorld->removeCollisionObject(gMapObject);

		gMapMesh = new btTriangleMesh(true, false);
		//Build new mesh
		btVector3 triangle[3];
		for (UInt64 i = 0; i < (vertices.size() - 3); i += 3)
		{
			const auto& vertex1 = vertices[i + 0];
			const auto& vertex2 = vertices[i + 1];
			const auto& vertex3 = vertices[i + 2];
			triangle[0].setValue(vertex1.x, vertex1.y, vertex1.z);
			triangle[1].setValue(vertex2.x, vertex2.y, vertex2.z);
			triangle[2].setValue(vertex3.x, vertex3.y, vertex3.z);

			gMapMesh->addTriangle(triangle[0], triangle[1], triangle[2], true);
		}
		gMapObject->setCollisionShape(new btBvhTriangleMeshShape(gMapMesh, true));

		gWorld->addCollisionObject(gMapObject);

		for (UInt64 i = 0; i < gWorld->getNumCollisionObjects(); i++)
		{
			auto ob = gWorld->getCollisionObjectArray()[i];
			ob->activate();
		}
	}
	void initColShapeSphere(btRigidBody* rigidBody, F32 radius)
	{
		delete rigidBody->getCollisionShape();
		btSphereShape* shape = new btSphereShape(radius);
		rigidBody->setCollisionShape(shape);
		recalculateIntertia(shape, rigidBody);
	}
	void initColShapePlane(btRigidBody* rigidBody, F32 x, F32 y, F32 z, F32 distance)
	{
		delete rigidBody->getCollisionShape();
		btStaticPlaneShape* shape = new btStaticPlaneShape(btVector3(x, y, z), distance);
		rigidBody->setCollisionShape(shape);
		recalculateIntertia(shape, rigidBody);
	}
	void initColShapeBox(btRigidBody* rigidBody, F32 x, F32 y, F32 z)
	{
		delete rigidBody->getCollisionShape();
		btBoxShape* shape = new btBoxShape(btVector3(x, y, z));
		rigidBody->setCollisionShape(shape);
		recalculateIntertia(shape, rigidBody);
	}
	void initCollisionShapeRuntime(btRigidBody* rigidBody, CollisionShapeType type)
	{
		switch (type)
		{
		case CollisionShapeType::SPHERE:
		{
			initColShapeSphere(rigidBody, 1);
			return;
		}
		case CollisionShapeType::BOX:
		{
			initColShapeBox(rigidBody, 0.5f, 0.5f, 0.5f);
			return;
		}
		}

	}
}


