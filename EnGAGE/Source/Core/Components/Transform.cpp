#include "pch.hpp"
#include "Transform.hpp"

#include "RigidBody.hpp"
#include "Core/ECS.hpp"
#include "Core/Math.hpp"
#include "Core/GameEngine.hpp"
#include "Core/Physics.hpp"
#include "Core/Messenger/Messenger.hpp"

#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

void Core::Transform::OnImGui(ComponentHeader* pHeader)
{
	static Int32 currentGizmoOperation = ImGuizmo::ROTATE;
	static Int32 currentGizmoMode = ImGuizmo::WORLD;

	Component* pTransform = (Component*)pHeader;
	RigidBody::Component* pRigidBody = (RigidBody::Component*)ECS::getComponent(pHeader->entity, ComponentType::RIGID_BODY);

	ImGui::RadioButton("Translate", &currentGizmoOperation, ImGuizmo::TRANSLATE); ImGui::SameLine();
	ImGui::RadioButton("Rotate", &currentGizmoOperation, ImGuizmo::ROTATE); ImGui::SameLine();
	ImGui::RadioButton("Scale", &currentGizmoOperation, ImGuizmo::SCALE);

	//Translate
	if (ImGui::DragFloat3("Translation", &pTransform->x, 0.1f) && pRigidBody)
	{
		if (pRigidBody)
		{
			pRigidBody->pRigidbody->getWorldTransform().setOrigin(btVector3(pTransform->x, pTransform->y, pTransform->z));
			Physics::updateRigidBody(pRigidBody->pRigidbody);
		}
	}

	//Rotate
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
			Physics::updateRigidBody(pRigidBody->pRigidbody);
		}
	}
	//Scale
	ImGui::DragFloat3("Scale", &pTransform->sx, 0.1f);


	auto& camera = GameEngine::getDebugCamera();
	auto proj = Math::calculateProj(camera);
	auto view = Math::calculateView(camera);

	Mat4x4 modelMat;
	modelMat = glm::translate(glm::mat4(1.0f), { pTransform->x, pTransform->y, pTransform->z });
	modelMat *= glm::toMat4(glm::quat{ pTransform->rw, pTransform->rx, pTransform->ry, pTransform->rz });
	modelMat = glm::scale(modelMat, { pTransform->sx, pTransform->sy, pTransform->sz });
	if (ImGuizmo::Manipulate(
		glm::value_ptr(view),
		glm::value_ptr(proj),
		(ImGuizmo::OPERATION)currentGizmoOperation, (ImGuizmo::MODE)currentGizmoMode,
		glm::value_ptr(modelMat), NULL, NULL))
	{
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMat), matrixTranslation, matrixRotation, matrixScale);
		pTransform->x = matrixTranslation[0];
		pTransform->y = matrixTranslation[1];
		pTransform->z = matrixTranslation[2];
		glm::quat rotation(glm::radians(Vec3{ matrixRotation[0], matrixRotation[1], matrixRotation[2] }));
		pTransform->rx = rotation.x;
		pTransform->ry = rotation.y;
		pTransform->rz = rotation.z;
		pTransform->rw = rotation.w;
		pTransform->sx = matrixScale[0];
		pTransform->sy = matrixScale[1];
		pTransform->sz = matrixScale[2];

		if (pRigidBody)
		{
			pRigidBody->pRigidbody->getWorldTransform().setRotation(btQuaternion(pTransform->rx, pTransform->ry, pTransform->rz, pTransform->rw));
			pRigidBody->pRigidbody->getWorldTransform().setOrigin(btVector3(pTransform->x, pTransform->y, pTransform->z));
			Physics::updateRigidBody(pRigidBody->pRigidbody);
		}
	}
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
