#include "pch.hpp"
#include "GameEngine.hpp"

#include "Memory.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Resource.hpp"

#include "Systems/Rendering.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Core
{
	namespace GameEngine
	{	
		static DynArr<Ref<GameObject>> gGameObjects;
		static Scope<Rendering> gRendering;

		void init(uint16_t width, uint16_t height, const String& title)
		{
			Log::init();
			Window::init(width, height, title);
			Input::init(Window::getRawWindow());
			ImGui::CreateContext();
			ImGui_ImplGlfw_InitForOpenGL(Window::getRawWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 330");

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			gRendering = createScope<Rendering>();
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

					Window::pollEvents();
				}


				//Render
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				ImGui::Begin("Debug");
				ImGui::DragFloat3("Cam pos", &gRendering->mCamera.x, 0.05f);
				ImGui::DragFloat3("Cam rot", &gRendering->mCamera.pitch, 1.05f);
				ImGui::End();

				gRendering->render();

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				Window::swapBuffers();
				double endTime = currentTime + secsPerRender;
				while (Window::getCurrentTime() < endTime) {
					using namespace std::chrono_literals;
					std::this_thread::sleep_for(1ms);
				}
			}
			Resource::cleanup();
			gRendering->cleanup();
			Window::destroy();
		}
		void addGameObject(Ref<GameObject> go)
		{
			gGameObjects.push_back(go);
			for (const auto& component : go->mComponents)
			{
				gRendering->addComponent(component);
			}
		}
		Rendering* getRenderer()
		{
			return gRendering.get();
		}
	}
}