#include "pch.hpp"
#include "Core/GameEngine.hpp"
#include "Core/Scene.hpp"


int main()
{
	using namespace Core;
	GameEngine::init(1600, 900, 1280, 720, "Hello world");
	Scene::loadScene("Resources/Scenes/Dump.lua");
	GameEngine::run();

}