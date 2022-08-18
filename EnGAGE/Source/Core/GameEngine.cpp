#include "pch.hpp"
#include "GameEngine.hpp"

#include "Window.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Resource.hpp"
#include "Core/ECS/ECS.hpp"
#include "Editor/Editor.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/DebugRenderer.hpp"
#include "Scripting.hpp"
#include "Core/Physics/Physics.hpp"
#include "Scene.hpp"
#include "Core/Messenger/Messenger.hpp"
#include "Map/Map.hpp"
#include "LuaHostFunctions.hpp"

namespace Core::GameEngine
{
	static void onFunctionKeys(const Message* pMessage)
	{
		if (auto keyPressedMessage = Messenger::messageCast<MessageType::KEY_PRESSED, KeyPressedMessage>(pMessage))
		{
			auto key = keyPressedMessage->keyCode;
#ifdef  EN_DEBUG
			//Enable editor
			if (key == InputCodes::KEY_F1)
			{
				Editor::toggle();
			}
#endif //  EN_DEBUG


			if (key == InputCodes::KEY_F11)
			{
				Window::toggleFullScreen();
			}


		}
	}

	static GameEngineData gData;

	void init(unsigned int width, unsigned int height, unsigned int fullScreenWidth, unsigned int fullScreenHeight, const String& title)
	{
		Log::init();
		Messenger::init();
		Window::init(width, height, fullScreenWidth, fullScreenHeight, title);
		Input::init();
		Editor::init(Window::getData().window, width, height);
		Map::init();

		ECS::init();

		Renderer::init(width, height);
		DebugRenderer::init();
		Physics::init();


	}

	void run()
	{
		constexpr unsigned int TPS = 128; //Tick per seconds

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
					Physics::update(secsPerUpdate);
					Scripting::update(secsPerUpdate);
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
				Editor::onMessage(pMessage);
				Renderer::onMessage(pMessage);

				if (!Editor::isEnabled())
				{
					Scripting::onMessage(pMessage);
				}
			}

			//Render

			if (Editor::isEnabled())
			{
				gData.currentCamera = &gData.debugCamera;
			}
			else
			{
				gData.currentCamera = &gData.mainCamera;
			}

			Renderer::render(*gData.currentCamera);
			Editor::render();
			DebugRenderer::render(*gData.currentCamera);

			Window::swapBuffers();

			ECS::updateRemovedEntities();
			Scene::checkForSceneSwitch();

		}
		clearResources();
		Map::shutdown();
		Editor::shutdown();
		Renderer::shutdown();
		DebugRenderer::shutdown();
		Physics::shutdown();
		Window::destroy();
		Messenger::shutdown();
	}
	void clearResources()
	{
		Editor::clear();
		Map::clear();
		Physics::clear();
		ECS::clear();
		Scripting::clear();
		Resource::clear();
	}
	GameEngineData& getEngineData()
	{
		return gData;
	}
	int luaUpdateCameraPerspective(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 9);
		auto& camera = gData.mainCamera;
		camera.mode = Camera::Mode::PERSPECTIVE;
		camera.x = (float)lua_tonumber(L, 1);
		camera.y = (float)lua_tonumber(L, 2);
		camera.z = (float)lua_tonumber(L, 3);
		camera.pitch = (float)lua_tonumber(L, 4);
		camera.yaw = (float)lua_tonumber(L, 5);
		camera.roll = (float)lua_tonumber(L, 6);
		camera.fov = (float)lua_tonumber(L, 7);
		camera.near = (float)lua_tonumber(L, 8);
		camera.far = (float)lua_tonumber(L, 9);

		return 0;
	}
}

int main()
{
	using namespace Core;
	GameEngine::init(1600, 900, 1600, 900, "YO");
	Scene::loadScene("Resources/Scenes/Test.lua");
	GameEngine::run();
}