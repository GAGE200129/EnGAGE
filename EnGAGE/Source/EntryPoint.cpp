#include "pch.hpp"
#include "Core/GameEngine.hpp"
#include "Core/Resource.hpp"

#include "Components/ModelRenderer.hpp"

int main()
{
	using namespace Core;
	GameEngine::init(1600, 900, "Hello world");

	auto go = createRef<Core::GameObject>();
	Core::GameObject::addComponent<Core::ModelRenderer>(go, Core::Resource::getModel("Resources/Models/NodeTest.glb"));

	GameEngine::addGameObject(go);


	GameEngine::run();
}