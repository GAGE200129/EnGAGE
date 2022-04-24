#include "pch.hpp"
#include "Core/GameEngine.hpp"
#include "Core/ECS.hpp"
#include "Core/Input.hpp"
#include "Core/Resource.hpp"
#include "Core/Script.hpp"
#include "Core/Scene.hpp"

int main()
{
	using namespace Core;
	GameEngine::init(1600, 900, 1280, 720, "Hello world");
	Scene::loadScene("Resources/Scenes/Dump2.lua");
	GameEngine::run();
}