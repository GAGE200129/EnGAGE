#include "pch.hpp"
#include "Core/GameEngine.hpp"
#include "Core/ECS.hpp"
#include "Core/Resource.hpp"

int main()
{
	using namespace Core;
	GameEngine::init(1600, 900, "Hello world");
	GameEngine::run();
}