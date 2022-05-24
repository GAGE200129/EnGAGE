#include "pch.hpp"
#include "GameEngine.hpp"

#include "Window.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Resource.hpp"
#include "ECS.hpp"
#include "Editor/Editor.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/DebugRenderer.hpp"
#include "Scripting.hpp"
#include "Physics.hpp"
#include "Scene.hpp"
#include "Messenger.hpp"

namespace Core::GameEngine
{
	Camera gMainCamera;
	Camera gDebugCamera;
	Camera* gCurrentCamera = nullptr;
	
	static void onFunctionKeys(const Message* pMessage)
	{
		if (auto keyPressedMessage = Messenger::messageCast<MessageType::KEY_PRESSED, KeyPressedMessage>(pMessage))
		{
			auto key = keyPressedMessage->keyCode;
			//Enable editor
			if (key == InputCodes::KEY_F1)
			{
				Messenger::queueMessage(MessageType::EDITOR_TOGGLE);
			}
		}
	}

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
	}

	void run()
	{
		constexpr unsigned int TPS = 120; //Tick per seconds

		const double secsPerUpdate = 1.0 / (double)TPS;
		double prevTime = Window::getCurrentTime();
		double steps = 0.0;
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
				else
				{
					Editor::update(F32(secsPerUpdate));
				}
			}

			//Process messages
			Window::pollEvents();
			Messenger::flushQueued();
			while (const Message* pMessage = Messenger::queryMessage())
			{
				onFunctionKeys(pMessage);
				Window::onMessage(pMessage);
				Editor::onMessage(pMessage);
				Input::onMessage(pMessage);
				Renderer::onMessage(pMessage);
				Physics::onMessage(pMessage);

				if (!Editor::isEnabled())
				{
					Scripting::onMessage(pMessage);
				}
			}

			//Render

			if (Editor::isEnabled())
			{
				gCurrentCamera = &gDebugCamera;
			}
			else
			{
				gCurrentCamera = &gMainCamera;
			}

			Renderer::render(*gCurrentCamera);
			Editor::render();
			DebugRenderer::render(*gCurrentCamera);
			
			Window::swapBuffers();

			ECS::updateRemovedEntities();
			Scene::checkForSceneSwitch();
		}
		clearResources();
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

