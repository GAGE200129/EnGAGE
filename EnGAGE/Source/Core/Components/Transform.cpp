#include "pch.hpp"
#include "Transform.hpp"

#include "RigidBody.hpp"
#include "Core/ECS.hpp"

void Core::Transform::OnImGui(ComponentHeader* pHeader)
{
	Component* pTransform = (Component*)pHeader;
	RigidBody::Component* pRigidBody = (RigidBody::Component*)ECS::getComponent(pHeader->entity, ComponentType::RIGID_BODY);

	if (ImGui::DragFloat3("Translation", &pTransform->x, 0.1f) && pRigidBody)
	{
		pRigidBody->pRigidbody->getWorldTransform().setOrigin(btVector3(pTransform->x, pTransform->y, pTransform->z));
		Core::PhysicsUpdateRigidBodyMessage message;
		message.body = pRigidBody->pRigidbody;
		Core::Messenger::recieveMessage(Core::MessageType::PHYSICS_UPDATE_RIGID_BODY, &message);
	}
	if (ImGui::DragFloat4("Rotation", &pTransform->rw, 0.1f))
	{
		float lengthSquared = pTransform->rw * pTransform->rw +
			pTransform->rx * pTransform->rx +
			pTransform->ry * pTransform->ry +
			pTransform->rz * pTransform->rz;
		if (lengthSquared != 0.0f)
		{
			float length = glm::sqrt(lengthSquared);
			pTransform->rw /= length;
			pTransform->rx /= length;
			pTransform->ry /= length;
			pTransform->rz /= length;
		}

		if (pRigidBody)
		{
			pRigidBody->pRigidbody->getWorldTransform().setRotation(btQuaternion(pTransform->rx, pTransform->ry, pTransform->rz, pTransform->rw));
			Core::PhysicsUpdateRigidBodyMessage message;
			message.body = pRigidBody->pRigidbody;
			Core::Messenger::recieveMessage(Core::MessageType::PHYSICS_UPDATE_RIGID_BODY, &message);
		}
	}

	ImGui::DragFloat3("Scale", &pTransform->sx, 0.1f);
}

void Core::Transform::OnSeralize(ComponentHeader* pComponent, std::ofstream& out, const String& entity)
{
	String componentName = entity + "_transform";
	out << componentName << " = _addComponent(" << entity << ", " << getHint().name << ")\n";
	Transform::Component* component = (Transform::Component*)pComponent;
	out << "_setPosition(" << componentName << ", " << component->x << ", " << component->y << ", " << component->z << ")\n";
	out << "_setRotation(" << componentName << ", " << component->rw << ", " << component->rx << ", " << component->ry << ", " << component->rz << ")\n";
	out << "_setScale(" << componentName << ", " << component->sx << ", " << component->sy << ", " << component->sz << ")\n";
}

void Core::Transform::Init(ComponentHeader* pHeader)
{
	Transform::Component* transformComponent = (Transform::Component*)pHeader;
	transformComponent->header.OnImGui = OnImGui;
	transformComponent->header.OnSeralize = OnSeralize;
	transformComponent->x = 0;
	transformComponent->y = 0;
	transformComponent->z = 0;
	transformComponent->rw = 1;
	transformComponent->rx = 0;
	transformComponent->ry = 0;
	transformComponent->rz = 0;
	transformComponent->sx = 1;
	transformComponent->sy = 1;
	transformComponent->sz = 1;
}
