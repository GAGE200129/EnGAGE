#include "pch.hpp"
#include "Editor.hpp"

#include "ECS.hpp"
#include "Resource.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/DebugRenderer.hpp"
#include "Scripting.hpp"
#include "GameEngine.hpp"
#include "Scene.hpp"
#include "Input.hpp"
#include "Physics.hpp"
#include "Messenger.hpp"
#include "Window.hpp"
#include "Scene.hpp"
#include "Math.hpp"
#include "DebugCamera.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Core::Editor
{
	enum class Mode
	{
		ENTITY,
		MAP
	};
	void processGameEngine();
	void processInspector(const ECS::EntitySignature* pEntity);
	void processSceneGraph();
	void processRenderer();
	void processResourceBrowser();
	void processMenuBar();
	void processConsole();
	void processMap();
	void renderWorldGrid();
	void renderMainCameraFrustum();
	void getCursorRay(Vec3& outPosition, Vec3& outRay);
	


	//static void placeWall(bool& placing, const Message* pMessage)
	//{
	//	if (auto keyPressedMessage = Messenger::messageCast<MessageType::KEY_PRESSED, KeyPressedMessage>(pMessage))
	//	{
	//		//Place wall
	//		if (keyPressedMessage->keyCode == InputCodes::KEY_F3)
	//		{
	//			placing = true;
	//		}
	//	}

	//	const ButtonPressedMessage* buttonPressed;
	//	if (placing && (buttonPressed = Messenger::messageCast<MessageType::BUTTON_PRESSED, ButtonPressedMessage>(pMessage)))
	//	{
	//		//Place wall
	//		if (buttonPressed->buttonCode == 0)
	//		{
	//			Vec3 position, ray;
	//			getCursorRay(position, ray);

	//			float t = -position.y / ray.y;

	//			Vec3I planeCursor = position + t * ray;
	//			Scene::addWall(planeCursor, planeCursor + Vec3I{0, 1, 0}, planeCursor + Vec3I{ 1, 1, 0 }, planeCursor + Vec3I{ 1, 0, 0 });
	//		}

	//		//Cancel
	//		if (buttonPressed->buttonCode == 1)
	//		{
	//			placing = false;
	//		}
	//	}
	//}

	static bool gEnabled = false;
	static bool gPrevCursorState = false;
	static DebugCamera gDebugCamera(GameEngine::getDebugCamera());
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
		if (pMessage->type == MessageType::EDITOR_TOGGLE)
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

		if (!gEnabled)
			return;

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
		else if (auto buttonPressed = Messenger::messageCast<MessageType::BUTTON_PRESSED, ButtonPressedMessage>(pMessage))
		{
			
		}

		gDebugCamera.onMessage(ImGui::GetIO().WantCaptureMouse, pMessage);
	}

	void update(F32 delta)
	{
		gDebugCamera.update(delta);
	}

	void render()
	{
#ifdef EN_DEBUG
		if (!gEnabled) return;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);

		processGameEngine();
		processRenderer();
		processResourceBrowser();
		processMenuBar();
		processConsole();
		if (gCurrentMode == Mode::ENTITY)
		{
			renderMainCameraFrustum();
			processSceneGraph();
		}
		else if (gCurrentMode == Mode::MAP)
		{
			processMap();
			renderWorldGrid();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
	}

	bool isEnabled()
	{
		return gEnabled;
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
	}

	static void renderMainCameraFrustum()
	{
		static constexpr Vec3 frustumColor = { 1, 1, 1 };
		const auto& camera = GameEngine::getMainCamera();

		auto frustum = Math::createFrustumPoints(camera, camera.near, camera.far);

		DebugRenderer::addLine(frustumColor, frustum.nbl, frustum.nbr);
		DebugRenderer::addLine(frustumColor, frustum.ntl, frustum.ntr);
		DebugRenderer::addLine(frustumColor, frustum.ntl, frustum.nbl);
		DebugRenderer::addLine(frustumColor, frustum.ntr, frustum.nbr);

		DebugRenderer::addLine(frustumColor, frustum.fbl, frustum.fbr);
		DebugRenderer::addLine(frustumColor, frustum.ftl, frustum.ftr);
		DebugRenderer::addLine(frustumColor, frustum.ftl, frustum.fbl);
		DebugRenderer::addLine(frustumColor, frustum.ftr, frustum.fbr);

		DebugRenderer::addLine(frustumColor, frustum.ntl, frustum.ftl);
		DebugRenderer::addLine(frustumColor, frustum.ntr, frustum.ftr);
		DebugRenderer::addLine(frustumColor, frustum.nbl, frustum.fbl);
		DebugRenderer::addLine(frustumColor, frustum.nbr, frustum.fbr);
	}
	static void renderWorldGrid()
	{
		constexpr Vec3 gridColor = { 0.3, 0.1, 0.1 };
		constexpr Int8 gridSize = 10;
		const auto& camera = GameEngine::getDebugCamera();
		const Vec3I camPos = { camera.x, camera.y, camera.z };


		for (Int8 i = -gridSize; i <= gridSize; i++)
		{
			DebugRenderer::addLine(gridColor, { i + camPos.x, 0, gridSize + camPos.z }, { i + camPos.x, 0, -gridSize + camPos.z });
		}
		for (Int8 i = -gridSize; i <= gridSize; i++)
		{
			DebugRenderer::addLine(gridColor, { gridSize + camPos.x, 0, i + camPos.z }, { -gridSize + camPos.x, 0, i + camPos.z });
		}


	}

	static void processMap()
	{
		ImGui::Begin("Map editor");
		if (ImGui::Button("New plane"))
		{
			
		}

		ImGui::End();
	}

	static void processGameEngine()
	{
		ImGui::Begin("GameEngine");

		if (ImGui::TreeNode("Memory")) {
			{// Entities
				char buf[50];
				unsigned int currentArrMemSize = sizeof(ECS::EntitySignature) * ECS::getEntityCount();
				unsigned int totalArrMemSize = sizeof(ECS::EntitySignature) * ECS::MAX_ENTITIES;
				sprintf(buf, "%d/%d bytes", currentArrMemSize, totalArrMemSize);
				ImGui::Text("Entity");
				ImGui::ProgressBar((float)currentArrMemSize / (float)totalArrMemSize, ImVec2(0.0f, 0.0f), buf);
				ImGui::Separator();
			}
			//Components
			for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
			{
				char buf[50];
				unsigned int currentArrMemSize = ECS::getComponentArrayMemorySize((ComponentType)i);
				unsigned int totalArrMemSize = ECS::MAX_COMPONENT_ARRAY_BUFFER_SIZE;
				sprintf(buf, "%d/%d bytes", currentArrMemSize, totalArrMemSize);
				ComponentHint data = getComponentHint((ComponentType)i);
				ImGui::Text(data.name);
				ImGui::ProgressBar((float)currentArrMemSize / (float)totalArrMemSize, ImVec2(0.0f, 0.0f), buf);
				ImGui::Separator();
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Messages"))
		{
			for (unsigned int i = 0; i < Messenger::getMessageCount(); i++)
			{
				const auto& data = getMessageData(Messenger::getMessages()[i].type);
				ImGui::Text("%s", data.name);
			}
			ImGui::TreePop();
		}
		ImGui::End();

	}

	static void processResourceBrowser()
	{
		static const char* modelPath = "Resources/Models";
		static const char* scriptPath = "Resources/Scripts";
		ImGui::Begin("ResourceBrowser");

		if (ImGui::TreeNode("Models"))
		{
			for (auto& entry : std::filesystem::recursive_directory_iterator(modelPath))
			{
				std::filesystem::path path = entry.path();
				String pathName = path.make_preferred().string();
				std::replace(pathName.begin(), pathName.end(), '\\', '/');

				ImGui::Selectable(pathName.c_str());
				if (ImGui::BeginDragDropSource())
				{
					ImGui::Text(pathName.c_str());
					ImGui::SetDragDropPayload("RESOURCE_MODEL", pathName.c_str(), pathName.size() + 1);
					ImGui::EndDragDropSource();
				}
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Scripts"))
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(scriptPath))
			{
				std::filesystem::path path = entry.path();
				String pathName = path.make_preferred().string();
				std::replace(pathName.begin(), pathName.end(), '\\', '/');

				ImGui::Selectable(pathName.c_str());
				if (ImGui::BeginDragDropSource())
				{
					ImGui::Text(pathName.c_str());
					ImGui::SetDragDropPayload("RESOURCE_SCRIPT", pathName.c_str(), pathName.size() + 1);
					ImGui::EndDragDropSource();
				}
			}

			ImGui::TreePop();
		}

		ImGui::End();
	}

	static void processSceneGraph()
	{

		static const ECS::EntitySignature* selectedEntity = nullptr;

		ImGui::Begin("SceneGraph");

		if (ImGui::BeginListBox("Entities"))
		{
			for (unsigned int i = 0; i < ECS::getEntityCount(); i++)
			{
				char buf[50];
				const auto& e = ECS::getEntitySignatures()[i];
				sprintf(buf, "Entity %llu", e.id);
				if (ImGui::Selectable(buf, selectedEntity == &e))
				{
					selectedEntity = &e;
				}
			}

			ImGui::EndListBox();
		}

		if (ImGui::Button("New"))
		{
			ECS::createEntity();
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete") && selectedEntity)
		{
			ECS::removeEntity(selectedEntity->id);
			selectedEntity = nullptr;
		}
		ImGui::End();

		processInspector(selectedEntity);
	}

	static void processMenuBar()
	{
		static const char* scenePath = "Resources/Scenes";
		bool openLoadPopup = false;
		bool openSavePopup = false;
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
				Core::GameEngine::clearResources();

			if (ImGui::MenuItem("Load"))
				openLoadPopup = true;



			if (ImGui::MenuItem("Save"))
				openSavePopup = true;


			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		if (openSavePopup)
			ImGui::OpenPopup("SaveScenePopup");
		if (ImGui::BeginPopup("SaveScenePopup"))
		{
			ImGui::InputText("Path", &Core::Scene::getLoadedSceneName());
			if (ImGui::Button("Save"))
			{
				Core::Scene::saveScene(Core::Scene::getLoadedSceneName());
			}
			ImGui::EndPopup();
		}

		if (openLoadPopup)
			ImGui::OpenPopup("LoadScenePopup");
		if (ImGui::BeginPopup("LoadScenePopup"))
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(scenePath))
			{
				std::filesystem::path path = entry.path();
				String pathName = path.make_preferred().string();
				std::replace(pathName.begin(), pathName.end(), '\\', '/');
				if (ImGui::Selectable(pathName.c_str()))
				{
					Core::Scene::loadScene(pathName);
				}
			}
			ImGui::EndPopup();
		}
	}

	static void processRenderer()
	{
		static bool drawAABB = false;
		static float renderScale = 1.0f;
		ImGui::Begin("Renderer");
		if (ImGui::Button("Toggle culling sphere"))
		{
			Core::Messenger::recieveMessage(Core::MessageType::RENDERER_TOGGLE_CULLING_SPHERE);
		}
		ImGui::Separator();
		if (ImGui::SliderFloat("Render Scale", &renderScale, 0.0f, 2.0f))
		{
			Core::RendererSetScaleMessage message{ renderScale };
			Core::Messenger::recieveMessage(Core::MessageType::RENDERER_SET_SCALE, &message);
		}


		ImGui::End();
	}

	static void processConsole()
	{
		ImGui::Begin("Console");

		//Console output
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
		const auto& stringLines = Core::Log::getStringLines();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

		for (const auto& line : stringLines) {
			ImVec4 color;
			bool has_color = false;

			if (has_color = (line.find("[info]") != std::string::npos))
			{
				color = ImVec4(0, 1, 0, 1);
			}
			else if (has_color = (line.find("[warn]") != std::string::npos))
			{
				color = ImVec4(1, 1, 0, 1);
			}

			else if (has_color = (line.find("[error]") != std::string::npos))
			{
				color = ImVec4(1, 0, 0, 1);
			}

			if (has_color)
				ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(line.c_str());
			if (has_color)
				ImGui::PopStyleColor();
		}

		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::End();
	}

	static void processInspector(const ECS::EntitySignature* pEntity)
	{
		using namespace ECS;
		ImGui::Begin("Inspector");
		//Process component
		if (pEntity) {
			for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
			{
				ComponentHeader* pHeader = (ComponentHeader*)getComponent(pEntity->id, (ComponentType)i);
				if (pHeader)
				{
					pHeader->OnImGui(pHeader);

					ImGui::PushID(i);
					if (ImGui::Button("Remove"))
					{
						removeComponent(pEntity->id, (ComponentType)i);
					}
					ImGui::PopID();
					ImGui::Separator();
				}
			}

			if (ImGui::Button("New"))
				ImGui::OpenPopup("NewComponent");

			if (ImGui::BeginPopup("NewComponent"))
			{
				for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
				{
					ComponentHint data = getComponentHint((ComponentType)i);
					if (ImGui::Selectable(data.name))
					{
						ECS::addComponent(pEntity->id, (ComponentType)i);
					}
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
}
