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

namespace Core
{
	namespace GameEngine
	{	
		void init(uint16_t width, uint16_t height, const String& title)
		{
			Log::init();
			Window::init(width, height, title);
			Input::init(Window::getRawWindow());
			Editor::init(Window::getRawWindow());

			ECS::init();
			
			Renderer::init();
			Physics::init();
		}

		void run()
		{
			constexpr unsigned int FPS = 120; 
			constexpr unsigned int TPS = 60; //Tick per seconds

			const double secsPerUpdate = 1.0 / (double)FPS;
			const double secsPerRender = 1.0 / (double)TPS;
			double prevTime = Window::getCurrentTime();
			double steps = 0.0;
			while (!Window::closeRequested()) {

				double currentTime = Window::getCurrentTime();
				double delta = currentTime - prevTime;
				prevTime = currentTime;

				steps += delta;

				Script::input();
				Input::update();
				Window::pollEvents();

				while (steps > secsPerUpdate)
				{
					steps -= secsPerUpdate;

					//Update
					Thread luaThread(Script::update, float(secsPerUpdate));
					Thread physicsThread(Physics::update, float(secsPerUpdate));
					
					luaThread.join();
					physicsThread.join();
				}
				

				//Render
				Renderer::render();
				if(!Input::cursorLocked())
					Editor::render();
				Window::swapBuffers();

				ECS::updateRemovedEntities();
				Scene::checkForSceneSwitch();
			}
			clearResources();
			Editor::shutdown();
			Renderer::shutdown();
			Physics::shutdown();
			Window::destroy();
		}
		void clearResources()
		{
			ECS::shutdown();
			Script::shutdown();
			Resource::shutdown();
		}

	}
}

