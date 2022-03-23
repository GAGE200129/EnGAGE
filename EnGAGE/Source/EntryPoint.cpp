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


	for (int i = 0; i < 300; i++)
	{
		unsigned modelTest = ECS::createEntity();
		ECS::addComponent(modelTest, ECS::ComponentType::MODEL_RENDERER);
		ECS::addComponent(modelTest, ECS::ComponentType::TRANSFORM);


		ECS::ModelRendererComponent* pModelRenderer = (ECS::ModelRendererComponent*)ECS::getComponent(modelTest, ECS::ComponentType::MODEL_RENDERER);
		pModelRenderer->pModel = Resource::getModel("Resources\\Models\\Cube.glb");

		ECS::addComponent(modelTest, ECS::ComponentType::SCRIPT);
		ECS::ScriptComponent* pModelScript = (ECS::ScriptComponent*)ECS::getComponent(modelTest, ECS::ComponentType::SCRIPT);
		Lua::loadFile(pModelScript->L, "Resources\\Scripts\\TestScript.lua");
	}

	GameEngine::run();
}