#include "pch.hpp"
#include "Core/GameEngine.hpp"

#include "Components/ModelRenderer.hpp"

int main()
{
	using namespace Core::GameEngine;
	init(1600, 900, "Hello world");

	auto go = createRef<Core::GameObject>();
	go->addComponent<Core::ModelRenderer>();

	addGameObject(go);


	run();
}