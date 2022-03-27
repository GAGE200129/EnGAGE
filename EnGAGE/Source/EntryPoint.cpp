#include "pch.hpp"
#include "Core/GameEngine.hpp"
#include "Core/ECS.hpp"
#include "Core/Input.hpp"
#include "Core/Resource.hpp"
#include "Core/Lua.hpp"

int main()
{
	using namespace Core;
	GameEngine::init(1600, 900, "Hello world");

	unsigned int camera = ECS::createEntity();
	ECS::addComponent(camera, ECS::ComponentType::SCRIPT);
	ECS::ScriptComponent* pCameraScript = (ECS::ScriptComponent*)ECS::getComponent(camera, ECS::ComponentType::SCRIPT);
	Lua::loadFile(pCameraScript->L, "Resources\\Scripts\\CameraScript.lua");

	unsigned int test = ECS::createEntity();
	ECS::addComponent(test, ECS::ComponentType::SCRIPT);
	ECS::ScriptComponent* pScript = (ECS::ScriptComponent*)ECS::getComponent(test, ECS::ComponentType::SCRIPT);
	Lua::loadFile(pScript->L, "Resources\\Scripts\\TestScript.lua");

	{
		unsigned int plane = ECS::createEntity();
		ECS::addComponent(plane, ECS::ComponentType::TRANSFORM);
		ECS::ModelRendererComponent* pPlaneModel = (ECS::ModelRendererComponent*)ECS::addComponent(plane, ECS::ComponentType::MODEL_RENDERER);
		pPlaneModel->pModel = Resource::getModel("Resources\\Models\\Plane.glb");
		ECS::RigidBodyComponent* pRigidBody = (ECS::RigidBodyComponent*)ECS::addComponent(plane, ECS::ComponentType::RIGID_BODY);
		pRigidBody->mass = 0.0f;
		pRigidBody->colliderType = ECS::ColliderType::PLANE;
		ECS::PlaneCollider* collider = (ECS::PlaneCollider*)pRigidBody->colliderData;
		collider->distance = 0;
		collider->x = 0;
		collider->y = 1;
		collider->z = 0;
	}


	{
		unsigned int plane = ECS::createEntity();
		ECS::addComponent(plane, ECS::ComponentType::TRANSFORM);
		ECS::ModelRendererComponent* pPlaneModel = (ECS::ModelRendererComponent*)ECS::addComponent(plane, ECS::ComponentType::MODEL_RENDERER);
		pPlaneModel->pModel = Resource::getModel("Resources\\Models\\Plane.glb");
		ECS::RigidBodyComponent* pRigidBody = (ECS::RigidBodyComponent*)ECS::addComponent(plane, ECS::ComponentType::RIGID_BODY);
		pRigidBody->mass = 0.0f;
		pRigidBody->colliderType = ECS::ColliderType::PLANE;
		ECS::PlaneCollider* collider = (ECS::PlaneCollider*)pRigidBody->colliderData;
		collider->distance = -10;
		collider->x = 1;
		collider->y = 0;
		collider->z = 0;
	}

	{
		unsigned int plane = ECS::createEntity();
		ECS::addComponent(plane, ECS::ComponentType::TRANSFORM);
		ECS::ModelRendererComponent* pPlaneModel = (ECS::ModelRendererComponent*)ECS::addComponent(plane, ECS::ComponentType::MODEL_RENDERER);
		pPlaneModel->pModel = Resource::getModel("Resources\\Models\\Plane.glb");
		ECS::RigidBodyComponent* pRigidBody = (ECS::RigidBodyComponent*)ECS::addComponent(plane, ECS::ComponentType::RIGID_BODY);
		pRigidBody->mass = 0.0f;
		pRigidBody->colliderType = ECS::ColliderType::PLANE;
		ECS::PlaneCollider* collider = (ECS::PlaneCollider*)pRigidBody->colliderData;
		collider->distance = -10;
		collider->x = -1;
		collider->y = 0;
		collider->z = 0;
	}

	{
		unsigned int plane = ECS::createEntity();
		ECS::addComponent(plane, ECS::ComponentType::TRANSFORM);
		ECS::ModelRendererComponent* pPlaneModel = (ECS::ModelRendererComponent*)ECS::addComponent(plane, ECS::ComponentType::MODEL_RENDERER);
		pPlaneModel->pModel = Resource::getModel("Resources\\Models\\Plane.glb");
		ECS::RigidBodyComponent* pRigidBody = (ECS::RigidBodyComponent*)ECS::addComponent(plane, ECS::ComponentType::RIGID_BODY);
		pRigidBody->mass = 0.0f;
		pRigidBody->colliderType = ECS::ColliderType::PLANE;
		ECS::PlaneCollider* collider = (ECS::PlaneCollider*)pRigidBody->colliderData;
		collider->distance = -10;
		collider->x = 0;
		collider->y = 0;
		collider->z = 1;
	}

	{
		unsigned int plane = ECS::createEntity();
		ECS::addComponent(plane, ECS::ComponentType::TRANSFORM);
		ECS::ModelRendererComponent* pPlaneModel = (ECS::ModelRendererComponent*)ECS::addComponent(plane, ECS::ComponentType::MODEL_RENDERER);
		pPlaneModel->pModel = Resource::getModel("Resources\\Models\\Plane.glb");
		ECS::RigidBodyComponent* pRigidBody = (ECS::RigidBodyComponent*)ECS::addComponent(plane, ECS::ComponentType::RIGID_BODY);
		pRigidBody->mass = 0.0f;
		pRigidBody->colliderType = ECS::ColliderType::PLANE;
		ECS::PlaneCollider* collider = (ECS::PlaneCollider*)pRigidBody->colliderData;
		collider->distance = -10;
		collider->x = 0;
		collider->y = 0;
		collider->z = -1;
	}

	GameEngine::run();
}