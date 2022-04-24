#include "pch.hpp"
#include "Core/GameEngine.hpp"
#include "Core/Scene.hpp"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

int main()
{
	using namespace Core;
	GameEngine::init(1600, 900, 1280, 720, "Hello world");
	Scene::loadScene("Resources/Scenes/Dump2.lua");
	GameEngine::run();

	btCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();

	delete collisionConfig;
}