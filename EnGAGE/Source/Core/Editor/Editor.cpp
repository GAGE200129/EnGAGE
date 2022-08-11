#include "pch.hpp"
#include "Editor.hpp"

#include "Core/ECS/ECS.hpp"
#include "Core/Resource.hpp"
#include "Core/Renderer/Renderer.hpp"
#include "Core/Renderer/DebugRenderer.hpp"
#include "Core/Scripting.hpp"
#include "Core/GameEngine.hpp"
#include "Core/Scene.hpp"
#include "Core/Input.hpp"
#include "Core/Physics/Physics.hpp"
#include "Core/Messenger/Messenger.hpp"
#include "Core/Window.hpp"
#include "Core/Scene.hpp"
#include "Core/Math.hpp"
#include "Core/Map/Map.hpp"
#include "GameEngineEditor.hpp"
#include "ResourceBrowser.hpp"
#include "RendererEditor.hpp"
#include "SceneGraphEditor.hpp"
#include "DebugCamera.hpp"
#include "MenuBar.hpp"
#include "Console.hpp"
#include "MapEditor.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>


namespace Core::Editor
{
	enum class Mode
	{
		ENTITY,
		MAP
	};


	static bool gEnabled = false;
	static bool gPrevCursorState = false;
	static Mode gCurrentMode = Mode::ENTITY;


	void init(GLFWwindow* pWindow, UInt32 width, UInt32 height)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	void shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void clear()
	{
		MapEditor::clear();
	}

	void onMessage(const Message* pMessage)
	{
		auto& io = ImGui::GetIO();
		if (!gEnabled || io.WantCaptureKeyboard)
			return;

		DebugCamera::onMessage(pMessage);

		if (auto keyPressedMessage = Messenger::messageCast<MessageType::KEY_PRESSED, KeyPressedMessage>(pMessage))
		{
			auto key = keyPressedMessage->keyCode;
			//Toggle editor mode
			if (key == InputCodes::KEY_TAB)
			{
				if (gCurrentMode == Mode::ENTITY) gCurrentMode = Mode::MAP;
				else if (gCurrentMode == Mode::MAP) gCurrentMode = Mode::ENTITY;
			}
		}
	}

	void update(F32 delta)
	{
		if (!gEnabled)
			return;

		DebugCamera::update(delta);
	}

	void render()
	{
		if (!gEnabled) 
			return;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);
		const WindowData& windowData = Window::getData();
		ImGuizmo::SetRect(0, 0, windowData.screenWidth, windowData.screenHeight);
		ImGuizmo::SetOrthographic(GameEngine::getEngineData().debugCamera.mode == Camera::Mode::ORTHOGRAPHIC);
		ImGuizmo::BeginFrame();

		//ImGui::Begin("GameView");
		//ImGui::Image((ImTextureID)Renderer::getGBuffer().getPost().getColorTex(), ImGui::GetContentRegionMax());
		//ImGui::End();

		GameEngineEditor::process();
		RendererEditor::process();
		ResourceBrowser::process();
		MenuBar::process();
		Console::process();
		if (gCurrentMode == Mode::ENTITY)
		{
			SceneGraphEditor::processSceneGraph();
			SceneGraphEditor::processInspector();
		}
		else if (gCurrentMode == Mode::MAP)
		{
			MapEditor::process();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	bool isEnabled()
	{
		return gEnabled;
	}

	void toggle()
	{
		gEnabled = !gEnabled;
		if (gEnabled)
		{
			gPrevCursorState = Input::cursorLocked();
			Input::enableCursor();
		}
		else // Restore cursor mode
		{
			if (gPrevCursorState) Input::disableCursor();
			else Input::enableCursor();
		}
	}
}
