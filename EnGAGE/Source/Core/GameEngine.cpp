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

	static bool gRunning = true;

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
				
				if (gRunning)
				{
					//Update
					Thread luaThread(Scripting::update, float(secsPerUpdate));
					Thread physicsThread(Physics::update, float(secsPerUpdate));

					luaThread.join();
					physicsThread.join();
				}

			}

			//Render
			Renderer::render();
			Editor::render();
			DebugRenderer::render();
			Window::swapBuffers();

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

				if (gRunning)
					Scripting::onMessage(pMessage);
#ifdef EN_DEBUG
				if (auto keyPressedMessage = Messenger::messageCast<MessageType::KEY_PRESSED, KeyPressedMessage>(pMessage))
				{
					if (keyPressedMessage->keyCode == InputCodes::KEY_F6)
					{
						gRunning = !gRunning;

						WindowRenamedMessage message;
						std::stringstream ss;
						ss << Window::getTitleName() << u8" | DEBUG MODE ! | " << (gRunning ? "Running" : "Paused");
						String name = ss.str();
						memset(message.name, 0, Messenger::BUFFER_SIZE);
						memcpy(message.name, name.c_str(), name.size());
						message.name[name.size()] = 0;
						Messenger::queueMessage(MessageType::WINDOW_RENAMED, &message);
					}
				}			
#endif
			}

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
		Physics::clearAllRigidBodies();
		ECS::shutdown();
		Scripting::shutdown();
		Resource::shutdown();
	}
}

