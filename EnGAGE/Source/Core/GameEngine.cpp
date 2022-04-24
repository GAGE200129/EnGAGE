#include "pch.hpp"
#include "GameEngine.hpp"

#include "Window.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Resource.hpp"
#include "ECS.hpp"
#include "Editor.hpp"
#include "Renderer.hpp"
#include "Script.hpp"
#include "Physics.hpp"
#include "Scene.hpp"
#include "Messaging.hpp"

namespace Core::GameEngine
{
	void init(unsigned int width, unsigned int height, unsigned int fullScreenWidth, unsigned int fullScreenHeight, const String& title)
	{
		Log::init();
		Messaging::init();
		Window::init(width, height, fullScreenWidth, fullScreenHeight, title);
		Input::init(Window::getRawWindow());
		Editor::init(Window::getRawWindow());

		ECS::init();

		Renderer::init(width, height);
		Physics::init();
	}

	void run()
	{
		constexpr unsigned int TPS = 120; //Tick per seconds

		const double secsPerUpdate = 1.0 / (double)TPS;
		double prevTime = Window::getCurrentTime();
		double steps = 0.0;
		while (!Window::closeRequested()) {

			double currentTime = Window::getCurrentTime();
			double delta = currentTime - prevTime;
			prevTime = currentTime;

			steps += delta;


			while (steps > secsPerUpdate)
			{
				steps -= secsPerUpdate;

				//Update
				Thread luaThread(Script::update, float(secsPerUpdate));
				Thread physicsThread(Physics::update, float(secsPerUpdate));

				luaThread.join();
				physicsThread.join();
			}

			Input::update();
			Window::pollEvents();

			//Render
			Renderer::render();
			Editor::render();
			Window::swapBuffers();

			Messaging::flushQueued();
			while (const Messaging::Message* pMessage = Messaging::queryMessage())
			{
				Window::onMessage(pMessage);
				Script::onMessage(pMessage);
				Editor::onMessage(pMessage);
				Input::onMessage(pMessage);
				Renderer::onMessage(pMessage);
			}

			ECS::updateRemovedEntities();
			Scene::checkForSceneSwitch();
		}
		clearResources();
		Editor::shutdown();
		Renderer::shutdown();
		Physics::shutdown();
		Window::destroy();
		Messaging::shutdown();
	}
	void clearResources()
	{
		ECS::shutdown();
		Script::shutdown();
		Resource::shutdown();
	}
}

