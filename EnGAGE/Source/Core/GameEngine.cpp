#include "pch.hpp"
#include "GameEngine.hpp"

#include "Window.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Resource.hpp"
#include "ECS.hpp"
#include "Editor.hpp"
#include "Renderer.hpp"
#include "DebugRenderer.hpp"
#include "Scripting.hpp"
#include "Physics.hpp"
#include "Scene.hpp"
#include "Messenger.hpp"

namespace Core::GameEngine
{
	Camera gMainCamera;
	Camera gDebugCamera;

	void init(unsigned int width, unsigned int height, unsigned int fullScreenWidth, unsigned int fullScreenHeight, const String& title)
	{
		Log::init();
		Messenger::init();
		Window::init(width, height, fullScreenWidth, fullScreenHeight, title);
		Input::init(Window::getRawWindow());
		Editor::init(Window::getRawWindow(), width, height);

		ECS::init();

		Renderer::init(width, height);
		DebugRenderer::init();
		Physics::init();
		Scene::init();
	}

	void run()
	{
		constexpr unsigned int TPS = 120; //Tick per seconds

		const double secsPerUpdate = 1.0 / (double)TPS;
		double prevTime = Window::getCurrentTime();
		double steps = 0.0;
		double timer = 0.0f;
		while (!Window::closeRequested())
		{

			double currentTime = Window::getCurrentTime();
			double delta = currentTime - prevTime;
			prevTime = currentTime;

			steps += delta;


			while (steps > secsPerUpdate)
			{
				steps -= secsPerUpdate;
				//Update

				if (!Editor::isEnabled())
				{
					Thread luaThread(Scripting::update, float(secsPerUpdate));
					Thread physicsThread(Physics::update, float(secsPerUpdate));

					luaThread.join();
					physicsThread.join();
				}
			}

			//Process messages
			Window::pollEvents();
			Messenger::flushQueued();
			while (const Message* pMessage = Messenger::queryMessage())
			{
				Window::onMessage(pMessage);
				Editor::onMessage(pMessage);
				Input::onMessage(pMessage);
				Renderer::onMessage(pMessage);
				Physics::onMessage(pMessage);
				Scripting::onMessage(pMessage);
			}

			//Render
			Scene::render();
			if (!Editor::isEnabled())
			{
				Renderer::render(gMainCamera);
			}
			else
			{
				Renderer::render(gDebugCamera);
			}
			Editor::render();
			DebugRenderer::render();
			Window::swapBuffers();

			ECS::updateRemovedEntities();
			Scene::checkForSceneSwitch();
		}
		clearResources();
		Scene::shutdown();
		Editor::shutdown();
		Renderer::shutdown();
		DebugRenderer::shutdown();
		Physics::shutdown();
		Window::destroy();
		Messenger::shutdown();
	}
	void clearResources()
	{
		Physics::clear();
		ECS::clear();
		Scripting::clear();
		Resource::clear();
	}
	Camera& getMainCamera()
	{
		return gMainCamera;
	}
	Camera& getDebugCamera()
	{
		return gDebugCamera;
	}
}

