#include "pch.hpp"
#include "Editor.hpp"

#include "Core/ECS.hpp"
#include "Core/Resource.hpp"
#include "Core/Renderer/Renderer.hpp"
#include "Core/Renderer/DebugRenderer.hpp"
#include "Core/Scripting.hpp"
#include "Core/GameEngine.hpp"
#include "Core/Scene.hpp"
#include "Core/Input.hpp"
#include "Core/Physics.hpp"
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

	void onMessage(const Message* pMessage)
	{
		auto io = ImGui::GetIO();
		if (!gEnabled && !io.WantCaptureMouse && !io.WantCaptureKeyboard)
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
		DebugCamera::update(delta);
	}

	void render()
	{
#ifdef EN_DEBUG
		if (!gEnabled) return;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);
		ImGuizmo::SetRect(0, 0, Window::getWidth(), Window::getHeight());
		ImGuizmo::SetOrthographic(GameEngine::getDebugCamera().mode == Camera::Mode::ORTHOGRAPHIC);
		ImGuizmo::BeginFrame();

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
#endif
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

	/*static void getCamFrontRay(Vec3& outPosition, Vec3& outRay)
	{
		auto& camera = GameEngine::getDebugCamera();

		outPosition.x = camera.x;
		outPosition.y = camera.y;
		outPosition.z = camera.z;

		auto x = ((Input::getX() / Window::getWidth()) - 0.5f) * 2;
		auto y = -((Input::getY() / Window::getHeight()) - 0.5f) * 2;
		auto projViewInverse = glm::inverse(Math::calculateProjectionView(camera));
		Vec4 rayStart = projViewInverse * Vec4(0, 0, 0, 1);
		rayStart *= 1.0f / rayStart.w;

		Vec4 rayEnd = projViewInverse * Vec4(0, 0, 1, 1);
		rayEnd *= 1.0f / rayEnd.w;

		outRay = glm::normalize(Vec3(rayEnd) - Vec3(rayStart));
	}

	static void getCursorRay(Vec3& outPosition, Vec3& outRay)
	{
		auto& camera = GameEngine::getDebugCamera();

		outPosition.x = camera.x;
		outPosition.y = camera.y;
		outPosition.z = camera.z;

		auto x = ((Input::getX() / Window::getWidth()) - 0.5f) * 2;
		auto y = -((Input::getY() / Window::getHeight()) - 0.5f) * 2;
		auto projViewInverse = glm::inverse(Math::calculateProjectionView(camera));
		Vec4 rayStart = projViewInverse * Vec4(x, y, 0, 1);
		rayStart *= 1.0f / rayStart.w;

		Vec4 rayEnd = projViewInverse * Vec4(x, y, 1, 1);
		rayEnd *= 1.0f / rayEnd.w;

		outRay = glm::normalize(Vec3(rayEnd) - Vec3(rayStart));
	}*/
}
