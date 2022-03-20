#include "pch.hpp"
#include "GameEngine.hpp"

#include "Memory.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Resource.hpp"
#include "ECS.hpp"

namespace Core
{
	namespace GameEngine
	{	
		void init(uint16_t width, uint16_t height, const String& title)
		{
			Log::init();
			Window::init(width, height, title);
			Input::init(Window::getRawWindow());
			Resource::init();
			ECS::init();
		}

		void run()
		{
			double secsPerUpdate = 1.0 / 60.0;
			double secsPerRender = 1.0 / 60.0;
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
					Input::update();
					Window::pollEvents();
				}


				//Render
				

				Window::swapBuffers();
				double endTime = currentTime + secsPerRender;
				while (Window::getCurrentTime() < endTime) {
					using namespace std::chrono_literals;
					std::this_thread::sleep_for(1ms);
				}
			}
			ECS::shutdown();
			Resource::cleanup();
			Window::destroy();
		}

		

		
	}
}
