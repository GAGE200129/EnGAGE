#include "pch.hpp"
#include "Transform.hpp"

#include "RigidBody.hpp"
#include "KinematicBody.hpp"
#include "Core/ECS/ECS.hpp"
#include "Core/Math.hpp"
#include "Core/GameEngine.hpp"
#include "Core/Physics/Physics.hpp"
#include "Core/Messenger/Messenger.hpp"
#include "Core/LuaHostFunctions.hpp"

#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

void Core::Transform::OnImGui(ComponentHeader* pHeader)
{
	static Int32 currentGizmoOperation = ImGuizmo::ROTATE;
	static Int32 currentGizmoMode = ImGuizmo::WORLD;

	Component* pTransform = (Component*)pHeader;
	KinematicBody::Component* kinematicBody = (KinematicBody::Component*)ECS::getComponent(pHeader->entity, ComponentType::KINEMATIC_BODY);
	RigidBody::Component* rigidBody = (RigidBody::Component*)ECS::getComponent(pHeader->entity, ComponentType::RIGID_BODY);
	
	btRigidBody* pRigidbody = nullptr;
	if (kinematicBody)
	{
		pRigidbody = kinematicBody->pKinematicBody;
	}
	else if (rigidBody)
	{
		pRigidbody = rigidBody->pRigidbody;
	}


	ImGui::RadioButton("Translate", &currentGizmoOperation, ImGuizmo::TRANSLATE); ImGui::SameLine();
	ImGui::RadioButton("Rotate", &currentGizmoOperation, ImGuizmo::ROTATE); ImGui::SameLine();
	ImGui::RadioButton("Scale", &currentGizmoOperation, ImGuizmo::SCALE);

	//Translate
	if (ImGui::DragFloat3("Translation", &pTransform->x, 0.1f) && pRigidbody)
	{
		if (pRigidbody)
		{
			pRigidbody->getWorldTransform().setOrigin(btVector3(pTransform->x, pTransform->y, pTransform->z));
			Physics::updateRigidBody(pRigidbody);
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

		if (pRigidbody)
		{
			pRigidbody->getWorldTransform().setRotation(btQuaternion(pTransform->rx, pTransform->ry, pTransform->rz, pTransform->rw));
			Physics::updateRigidBody(pRigidbody);
		}
	}
	//Scale
	ImGui::DragFloat3("Scale", &pTransform->sx, 0.1f);


	auto& camera = GameEngine::getEngineData().debugCamera;
	auto proj = Math::calculateProj(camera);
	auto view = Math::calculateView(camera);

	Mat4x4 modelMat;
	modelMat = glm::translate(glm::mat4(1.0f), { pTransform->x, pTransform->y, pTransform->z });
	modelMat *= glm::toMat4(glm::quat{ pTransform->rw, pTransform->rx, pTransform->ry, pTransform->rz });
	modelMat = glm::scale(modelMat, { pTransform->sx, pTransform->sy, pTransform->sz });
	if (ImGuizmo::Manipulate(
		glm::value_ptr(view),
		glm::value_ptr(proj),
		(ImGuizmo::OPERATION)currentGizmoOperation,
		(ImGuizmo::MODE)currentGizmoMode,
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

		if (pRigidbody)
		{
			pRigidbody->getWorldTransform().setRotation(btQuaternion(pTransform->rx, pTransform->ry, pTransform->rz, pTransform->rw));
			pRigidbody->getWorldTransform().setOrigin(btVector3(pTransform->x, pTransform->y, pTransform->z));
			Physics::updateRigidBody(pRigidbody);
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

int Core::Transform::luaSetPosition(lua_State* L)
{
	CHECK_NUM_ARGS(L, 4);
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TNUMBER);
	CHECK_ARG(L, 3, LUA_TNUMBER);
	CHECK_ARG(L, 4, LUA_TNUMBER);

	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
	Transform::Component* transform = (Transform::Component*)header;
	transform->x = (float)lua_tonumber(L, 2);
	transform->y = (float)lua_tonumber(L, 3);
	transform->z = (float)lua_tonumber(L, 4);

	return 0;
}

int Core::Transform::luaSetRotation(lua_State* L)
{
	CHECK_NUM_ARGS(L, 5);
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TNUMBER);
	CHECK_ARG(L, 3, LUA_TNUMBER);
	CHECK_ARG(L, 4, LUA_TNUMBER);
	CHECK_ARG(L, 5, LUA_TNUMBER);

	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
	Transform::Component* transform = (Transform::Component*)header;
	transform->rw = (float)lua_tonumber(L, 2);
	transform->rx = (float)lua_tonumber(L, 3);
	transform->ry = (float)lua_tonumber(L, 4);
	transform->rz = (float)lua_tonumber(L, 5);
	return 0;
}

int Core::Transform::luaSetScale(lua_State* L)
{
	CHECK_NUM_ARGS(L, 4);
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TNUMBER);
	CHECK_ARG(L, 3, LUA_TNUMBER);
	CHECK_ARG(L, 4, LUA_TNUMBER);

	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
	Transform::Component* transform = (Transform::Component*)header;
	transform->sx = (float)lua_tonumber(L, 2);
	transform->sy = (float)lua_tonumber(L, 3);
	transform->sz = (float)lua_tonumber(L, 4);
	return 0;
}

int Core::Transform::luaGetTransform(lua_State* L)
{
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
	Transform::Component* pTransform = (Transform::Component*)header;

	lua_pushnumber(L, pTransform->x);
	lua_pushnumber(L, pTransform->y);
	lua_pushnumber(L, pTransform->z);

	lua_pushnumber(L, pTransform->rw);
	lua_pushnumber(L, pTransform->rx);
	lua_pushnumber(L, pTransform->ry);
	lua_pushnumber(L, pTransform->rz);

	lua_pushnumber(L, pTransform->sx);
	lua_pushnumber(L, pTransform->sy);
	lua_pushnumber(L, pTransform->sz);
	return 10;
}
