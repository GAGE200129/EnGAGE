#include "pch.hpp"
#include "RigidBody.hpp"

#include "Core/Physics.hpp"

void Core::RigidBody::OnImGui(ComponentHeader* pHeader)
{
	using namespace Physics;
	Component* pComponent = (Component*)pHeader;
	btRigidBody* pRigidBody = pComponent->pRigidbody;

	if (pComponent->collisionShapeType != (unsigned int)CollisionShapeType::EMPTY)
	{
		float mass = pRigidBody->getMass();
		if (ImGui::DragFloat("Mass", &mass, 0.1f, 0.0f, 500.0f))
		{
			btVector3 inertia;
			pRigidBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
			pRigidBody->setMassProps(mass, inertia);
			pRigidBody->updateInertiaTensor();
			pRigidBody->activate();

			Core::PhysicsUpdateRigidBodyMessage message;
			message.body = pRigidBody;
			Core::Messenger::recieveMessage(Core::MessageType::PHYSICS_UPDATE_RIGID_BODY, &message);
		}

		btVector3 btVel = pRigidBody->getLinearVelocity();
		float vel[] = { btVel.x(), btVel.y(), btVel.z() };
		if (ImGui::DragFloat3("Velocity", vel, 0.1f))
		{
			pRigidBody->setLinearVelocity(btVector3(vel[0], vel[1], vel[2]));
			pRigidBody->activate();
		}
	}
	const char* currentColliderName = getCollisionShapeName((CollisionShapeType)pComponent->collisionShapeType);
	if (ImGui::BeginCombo("Collision shape type", currentColliderName))
	{
		for (unsigned int i = 0; i < (unsigned int)CollisionShapeType::COUNT; i++)
		{
			const char* colliderName = getCollisionShapeName((CollisionShapeType)i);
			if (ImGui::Selectable(colliderName))
			{
				pComponent->collisionShapeType = i;
				initCollisionShapeRuntime(pRigidBody, (CollisionShapeType)i);
			}
		}
		ImGui::EndCombo();
	}
}

void Core::RigidBody::OnSeralize(ComponentHeader* pComponent, std::ofstream& out, const String& entity)
{
	String componentName = entity + "_rigidBody";
	out << componentName << " = _addComponent(" << entity << ", " << getHint().name << ")\n";
	RigidBody::Component* component = (RigidBody::Component*)pComponent;
	out << "_setRigidBody(" << componentName << ", " << component->pRigidbody->getMass() << ", " << (UInt32)component->collisionShapeType << ", ";

	auto rigidBody = component->pRigidbody;
	switch ((Physics::CollisionShapeType)component->collisionShapeType)
	{
	case Physics::CollisionShapeType::BOX:
	{
		btBoxShape* shape = (btBoxShape*)(rigidBody->getCollisionShape());
		btVector3 halfExtend = shape->getHalfExtentsWithoutMargin();
		out << halfExtend.x() << ", " << halfExtend.y() << ", " << halfExtend.z() << ")\n";
		break;
	}
	case Physics::CollisionShapeType::SPHERE:
	{
		btSphereShape* shape = (btSphereShape*)(rigidBody->getCollisionShape());
		float radius = shape->getRadius();
		out << radius << ")\n";
		break;
	}
	case Physics::CollisionShapeType::PLANE:
	{
		btStaticPlaneShape* shape = (btStaticPlaneShape*)(rigidBody->getCollisionShape());
		auto normal = shape->getPlaneNormal();
		auto distance = shape->getPlaneConstant();
		out << normal.x() << ", " << normal.y() << ", " << normal.z() << ", " << distance << ")\n";
		break;
	}
	}
}

void Core::RigidBody::Destroy(ComponentHeader* pHeader)
{
	RigidBody::Component* component = (RigidBody::Component*)pHeader;
	if (component)
	{
		RemoveRigidBodyMessage message;
		message.body = component->pRigidbody;
		Messenger::recieveMessage(MessageType::REMOVE_RIGID_BODY, &message);
	}
}

void Core::RigidBody::Init(ComponentHeader* pHeader)
{
	RigidBody::Component* component = (RigidBody::Component*)pHeader;
	component->header.OnImGui = OnImGui;
	component->header.OnSeralize = OnSeralize;
	component->header.Destroy = Destroy;
	component->pRigidbody = Physics::newRigidBody(pHeader->entity);
	component->collisionShapeType = 0;
}
