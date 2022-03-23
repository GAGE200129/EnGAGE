#include "pch.hpp"
#include "Editor.hpp"

#include "ECS.hpp"
#include "Resource.hpp"
#include "Renderer.hpp"
#include "Lua.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

static void processGameEngine();
static void processSceneGraph();
static void processComponent(Core::ECS::ComponentType type, Core::ECS::ComponentHeader* pHeader);
static void processInspector(const Core::ECS::EntitySignature* pEntity);
static void processRenderer();
static void processResourceBrowser();

void Core::Editor::init(GLFWwindow* pWindow)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Core::Editor::shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Core::Editor::render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);

	//ImGui::ShowDemoWindow();
	processGameEngine();
	processSceneGraph();
	processRenderer();
	processResourceBrowser();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void processGameEngine()
{
	using namespace Core;
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
		for (unsigned int i = 0; i < (unsigned int)ECS::ComponentType::COUNT; i++)
		{
			char buf[50];
			unsigned int currentArrMemSize = ECS::getComponentArrayMemorySize((ECS::ComponentType)i);
			unsigned int totalArrMemSize = ECS::MAX_COMPONENT_ARRAY_BUFFER_SIZE;
			sprintf(buf, "%d/%d bytes", currentArrMemSize, totalArrMemSize);
			ECS::ComponentData data = ECS::getComponentData((ECS::ComponentType)i);
			ImGui::Text(data.name);
			ImGui::ProgressBar((float)currentArrMemSize / (float)totalArrMemSize, ImVec2(0.0f, 0.0f), buf);
			ImGui::Separator();
		}

		ImGui::TreePop();
	}
	ImGui::End();
}

static void processSceneGraph()
{
	using namespace Core;

	static const ECS::EntitySignature* selectedEntity = nullptr;

	ImGui::Begin("SceneGraph");

	if (ImGui::BeginListBox("Entities"))
	{
		for (unsigned int i = 0; i < ECS::getEntityCount(); i++)
		{
			char buf[50];
			const auto& e = ECS::getEntitySignatures()[i];
			sprintf(buf, "Entity %d", e.id);
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

static void processComponent(Core::ECS::ComponentType type, Core::ECS::ComponentHeader* pHeader)
{
	using namespace Core::ECS;
	switch (type)
	{
	case ComponentType::NAME:
	{
		NameComponent* pName = (NameComponent*)pHeader;
		ImGui::InputText("Name", pName->name, MAX_NAME_SIZE);
		break;
	}
	case ComponentType::TRANSFORM:
	{
		TransformComponent* pTransform = (TransformComponent*)pHeader;
		ImGui::DragFloat3("Translation", &pTransform->x, 0.1f);
		ImGui::DragFloat4("Rotation", &pTransform->rw, 0.1f);
		ImGui::DragFloat3("Scale", &pTransform->sx, 0.1f);
		break;
	}
	case Core::ECS::ComponentType::MODEL_RENDERER:
	{
		ModelRendererComponent* pModel = (ModelRendererComponent*)pHeader;

		ImGui::Text("Model: %p", pModel->pModel);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("RESOURCE_MODEL"))
			{
				pModel->pModel = Core::Resource::getModel((const char*)payload->Data);
			}
			ImGui::EndDragDropTarget();
		}
		break;
	}
	case Core::ECS::ComponentType::SCRIPT:
	{
		ScriptComponent* pScript = (ScriptComponent*)pHeader;

		ImGui::Text("Script: %p", pScript->L);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_SCRIPT"))
			{
				Core::Lua::loadFile(pScript->L, String((char*)payload->Data));
			}
			ImGui::EndDragDropTarget();
		}
		break;
	}

	default:
		break;
	};

}

void processInspector(const Core::ECS::EntitySignature* pEntity)
{
	using namespace Core::ECS;
	using namespace Core;


	ImGui::Begin("Inspector");
	//Process component
	if (pEntity) {
		for (unsigned int i = 0; i < (unsigned int)ECS::ComponentType::COUNT; i++)
		{
			ECS::ComponentHeader* pHeader = (ECS::ComponentHeader*)ECS::getComponent(pEntity->id, (ECS::ComponentType)i);
			if (pHeader)
			{
				processComponent((ECS::ComponentType)i, pHeader);

				ImGui::PushID(i);
				if (ImGui::Button("Remove"))
				{	
					ECS::removeComponent(pEntity->id, (ECS::ComponentType)i);
				}
				ImGui::PopID();
				ImGui::Separator();
			}
		}

		if (ImGui::Button("New"))
			ImGui::OpenPopup("NewComponent");

		if (ImGui::BeginPopup("NewComponent"))
		{
			for (unsigned int i = 0; i < (unsigned int)ECS::ComponentType::COUNT; i++)
			{
				ECS::ComponentData data = ECS::getComponentData((ECS::ComponentType)i);
				if (ImGui::Selectable(data.name))
				{
					ECS::addComponent(pEntity->id, (ECS::ComponentType)i);
				}
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

static void processRenderer()
{
	ImGui::Begin("Renderer");
	if (ImGui::TreeNode("Camera"))
	{
		ImGui::DragFloat3("position", &Core::Renderer::getCamera().x, 0.1f);
		ImGui::DragFloat3("rotation", &Core::Renderer::getCamera().pitch, 0.1f);
		ImGui::TreePop();
	}
	ImGui::End();
}

static void processResourceBrowser()
{
	static const char* modelPath = "Resources\\Models";
	static const char* scriptPath = "Resources\\Scripts";
	ImGui::Begin("ResourceBrowser");

	if(ImGui::TreeNode("Models"))
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(modelPath))
		{
			auto& pathName = entry.path().string();
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
			auto& pathName = entry.path().string();
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
