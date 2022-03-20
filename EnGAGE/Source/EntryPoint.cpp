#include "pch.hpp"
#include "Core/GameEngine.hpp"
#include "Core/ECS.hpp"
#include "Core/Resource.hpp"



int main()
{
	using namespace Core;
	using namespace Core::ECS;
	GameEngine::init(1600, 900, "Hello world");

	Entity entity =  createEntity();
	Entity entity2 =  createEntity();

	Core::TransformComponent test = {};
	Core::ModelRendererComponent test2 = {};
	test.x = 10;
	test.y = 10;
	test.z = 10;
	test.sx = 10;
	test.sy = 10;
	test.sz = 10;
	constructComponent(&entity, Core::ComponentType::TRANSFORM, &test);

	test.x = 30;
	test.y = 30;
	test.z = 30;
	test.sx = 30;
	test.sy = 30;
	test.sz = 30;
	constructComponent(&entity2, Core::ComponentType::TRANSFORM, &test);
	constructComponent(&entity2, Core::ComponentType::MODEL_RENDERER, &test2);

	removeEntity(&entity2);

	GameEngine::run();
}