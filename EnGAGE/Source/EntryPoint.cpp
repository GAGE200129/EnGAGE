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

	{
		unsigned int plane = ECS::createEntity();
		ECS::addComponent(plane, ECS::ComponentType::TRANSFORM);
		ECS::addComponent(plane, ECS::ComponentType::MODEL_RENDERER);
		ECS::addComponent(plane, ECS::ComponentType::RIGID_BODY);
		ECS::ModelRendererComponent* pPlaneModel = (ECS::ModelRendererComponent*)ECS::getComponent(plane, ECS::ComponentType::MODEL_RENDERER);
		pPlaneModel->pModel = Resource::getModel("Resources\\Models\\Plane.glb");
		ECS::RigidBodyComponent* pRigidBody = (ECS::RigidBodyComponent*)ECS::getComponent(plane, ECS::ComponentType::RIGID_BODY);
		pRigidBody->mass = 0.0f;
		pRigidBody->colliderType = ECS::ColliderType::PLANE;
		ECS::PlaneCollider collider;
		collider.distance = 0;
		collider.x = 0;
		collider.y = 1;
		collider.z = 0;
		memcpy(pRigidBody->colliderData, &collider, sizeof(collider));
	}
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dist(-5, 5);
	std::uniform_real_distribution<float> dist2(10, 30);

	for (int i = 0; i < 20; i++)
	{
		unsigned int modelTest = ECS::createEntity();
		ECS::addComponent(modelTest, ECS::ComponentType::TRANSFORM);
		ECS::TransformComponent* pTransform = (ECS::TransformComponent*)ECS::getComponent(modelTest, ECS::ComponentType::TRANSFORM);
		pTransform->x = dist(rng);
		pTransform->y = dist2(rng);
		pTransform->z = dist(rng);

		ECS::addComponent(modelTest, ECS::ComponentType::MODEL_RENDERER);
		ECS::ModelRendererComponent* pModelRenderer = (ECS::ModelRendererComponent*)ECS::getComponent(modelTest, ECS::ComponentType::MODEL_RENDERER);
		pModelRenderer->pModel = Resource::getModel("Resources\\Models\\Sphere.glb");

		//ECS::addComponent(modelTest, ECS::ComponentType::SCRIPT);
		//ECS::ScriptComponent* pModelScript = (ECS::ScriptComponent*)ECS::getComponent(modelTest, ECS::ComponentType::SCRIPT);
		//Lua::loadFile(pModelScript->L, "Resources\\Scripts\\TestScript.lua");

		ECS::addComponent(modelTest, ECS::ComponentType::RIGID_BODY);
		ECS::RigidBodyComponent* pRigidBody = (ECS::RigidBodyComponent*)ECS::getComponent(modelTest, ECS::ComponentType::RIGID_BODY);
		pRigidBody->velocity = { 0, 0, 0 };
		pRigidBody->mass = 1;
		pRigidBody->colliderType = ECS::ColliderType::SPHERE;
		ECS::SphereCollider collider;
		collider.radius = 1.0f;
		memcpy(pRigidBody->colliderData, &collider, sizeof(collider));
	}

	GameEngine::run();
}