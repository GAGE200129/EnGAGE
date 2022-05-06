#include "pch.hpp"
#include "Editor.hpp"

#include "ECS.hpp"
#include "Resource.hpp"
#include "Renderer.hpp"
#include "DebugRenderer.hpp"
#include "Script.hpp"
#include "GameEngine.hpp"
#include "Scene.hpp"
#include "Input.hpp"
#include "Physics.hpp"
#include "Messenger.hpp"
#include "Window.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>

static void processGameEngine();
static void processSceneGraph();
static void processComponent(unsigned int entity, Core::ComponentType type, Core::ComponentHeader* pHeader);
static void processInspector(const Core::ECS::EntitySignature* pEntity);
static void processRenderer();
static void processResourceBrowser();
static void processMenuBar();
static void processConsole();

static bool gEnabled = false;

void Core::Editor::init(GLFWwindow* pWindow, int width, int height)
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

void Core::Editor::onMessage(const Message* pMessage)
{
	if (auto keyPressedMessage = Messenger::messageCast<MessageType::KEY_PRESSED, KeyPressedMessage>(pMessage))
	{
		if (keyPressedMessage->keyCode == InputCodes::KEY_F5)
		{
			gEnabled = !gEnabled;
		}
	}
}

void Core::Editor::render()
{
#ifdef EN_DEBUG
	if (gEnabled)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);

		processGameEngine();
		processSceneGraph();
		processRenderer();
		processResourceBrowser();
		processMenuBar();
		processConsole();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
#endif
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
		for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
		{
			char buf[50];
			unsigned int currentArrMemSize = ECS::getComponentArrayMemorySize((ComponentType)i);
			unsigned int totalArrMemSize = ECS::MAX_COMPONENT_ARRAY_BUFFER_SIZE;
			sprintf(buf, "%d/%d bytes", currentArrMemSize, totalArrMemSize);
			ComponentData data = getComponentData((ComponentType)i);
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


static void processComponent(unsigned int entity, Core::ComponentType type, Core::ComponentHeader* pHeader)
{
	using namespace Core;
	using namespace Core::Physics;
	switch (type)
	{
	case ComponentType::NAME:
	{
		NameComponent* pName = (NameComponent*)pHeader;
		ImGui::InputText("Name", pName->name, MAX_NAME_SIZE);
		break;
	}
	case ComponentType::RIGID_BODY:
	{
		RigidBodyComponent* pComponent = (RigidBodyComponent*)pHeader;
		btRigidBody* pRigidBody = pComponent->pRigidbody;

		if (pComponent->collisionShapeType != (unsigned int)CollisionShapeType::EMPTY)
		{
			float mass = pRigidBody->getMass();
			if (ImGui::DragFloat("Mass", &mass, 0.1f, 0.0f, 500.0f))
			{
				btVector3 inertia;
				pRigidBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
				pRigidBody->setMassProps(mass, inertia);
				pRigidBody->updateInertiaTensor();
				pRigidBody->activate();

				Core::PhysicsUpdateRigidBodyMessage message;
				message.body = pRigidBody;
				Core::Messenger::recieveMessage(Core::MessageType::PHYSICS_UPDATE_RIGID_BODY, &message);
			}

			btVector3 btVel = pRigidBody->getLinearVelocity();
			float vel[] = { btVel.x(), btVel.y(), btVel.z() };
			if (ImGui::DragFloat3("Velocity", vel, 0.1f))
			{
				pRigidBody->setLinearVelocity(btVector3(vel[0], vel[1], vel[2]));
				pRigidBody->activate();
			}
		}
		const char* currentColliderName = getCollisionShapeName((CollisionShapeType)pComponent->collisionShapeType);
		if (ImGui::BeginCombo("Collision shape type", currentColliderName))
		{
			for (unsigned int i = 0; i < (unsigned int)CollisionShapeType::COUNT; i++)
			{
				const char* colliderName = getCollisionShapeName((CollisionShapeType)i);
				if (ImGui::Selectable(colliderName))
				{
					pComponent->collisionShapeType = i;
					initCollisionShapeRuntime(pRigidBody, (CollisionShapeType)i);
				}
			}
			ImGui::EndCombo();
		}

		break;
	}
	case ComponentType::TRANSFORM:
	{
		TransformComponent* pTransform = (TransformComponent*)pHeader;
		RigidBodyComponent* pRigidBody = (RigidBodyComponent * )ECS::getComponent(entity, ComponentType::RIGID_BODY);

		if (ImGui::DragFloat3("Translation", &pTransform->x, 0.1f) && pRigidBody)
		{
			pRigidBody->pRigidbody->getWorldTransform().setOrigin(btVector3(pTransform->x, pTransform->y, pTransform->z));
			Core::PhysicsUpdateRigidBodyMessage message;
			message.body = pRigidBody->pRigidbody;
			Core::Messenger::recieveMessage(Core::MessageType::PHYSICS_UPDATE_RIGID_BODY, &message);
		}
		if (ImGui::DragFloat4("Rotation", &pTransform->rw, 0.1f))
		{
			float lengthSquared = pTransform->rw * pTransform->rw +
				pTransform->rx * pTransform->rx +
				pTransform->ry * pTransform->ry +
				pTransform->rz * pTransform->rz;
			if (lengthSquared != 0.0f)
			{
				float length = glm::sqrt(lengthSquared);
				pTransform->rw /= length;
				pTransform->rx /= length;
				pTransform->ry /= length;
				pTransform->rz /= length;
			}

			if (pRigidBody)
			{
				pRigidBody->pRigidbody->getWorldTransform().setRotation(btQuaternion(pTransform->rx, pTransform->ry, pTransform->rz, pTransform->rw));
				Core::PhysicsUpdateRigidBodyMessage message;
				message.body = pRigidBody->pRigidbody;
				Core::Messenger::recieveMessage(Core::MessageType::PHYSICS_UPDATE_RIGID_BODY, &message);
			}
		}

		ImGui::DragFloat3("Scale", &pTransform->sx, 0.1f);

		
		break;
	}
	case ComponentType::MODEL_RENDERER:
	{
		ModelRendererComponent* pModel = (ModelRendererComponent*)pHeader;

		ImGui::Text("Model: %p", pModel->pModel);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_MODEL"))
			{
				pModel->pModel = Core::Resource::getModel((const char*)payload->Data);
				strcpy(pModel->modelPath, (const char*)payload->Data);
			}
			ImGui::EndDragDropTarget();
		}
		break;
	}
	case ComponentType::SCRIPT:
	{
		ScriptComponent* pScript = (ScriptComponent*)pHeader;

		ImGui::Text("Script: %p", pScript->L);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_SCRIPT"))
			{
				Core::Script::loadFile(pScript->L, String((char*)payload->Data));
			}
			ImGui::EndDragDropTarget();
		}
		break;
	}
	case ComponentType::DIRECTIONAL_LIGHT:
	{
		DirectionalLightComponent* pLight = (DirectionalLightComponent*)pHeader;
		ImGui::DragFloat3("Direction", &pLight->direction.x, 0.1f, -1, 1);
		ImGui::DragFloat3("Color", &pLight->color.x, 0.1f, 0, 1);
		ImGui::DragFloat("Intensity", &pLight->intensity, 0.1f, 0.0f, 1.0f);
		break;
	}

	};

}

static void processInspector(const Core::ECS::EntitySignature* pEntity)
{
	using namespace Core::ECS;
	using namespace Core;


	ImGui::Begin("Inspector");
	//Process component
	if (pEntity) {
		for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
		{
			ComponentHeader* pHeader = (ComponentHeader*)getComponent(pEntity->id, (ComponentType)i);
			if (pHeader)
			{
				processComponent(pEntity->id, (ComponentType)i, pHeader);

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
				ComponentData data = getComponentData((ComponentType)i);
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

static void processRenderer()
{
	static bool drawAABB = false;
	ImGui::Begin("Renderer");
	if (ImGui::TreeNode("Camera"))
	{
		ImGui::DragFloat3("position", &Core::Renderer::getCamera().x, 0.1f);
		ImGui::DragFloat3("rotation", &Core::Renderer::getCamera().pitch, 0.1f);
		ImGui::DragFloat("fov", &Core::Renderer::getCamera().fov, 0.1f, 1.0f, 180.0f);
		ImGui::DragFloat("near", &Core::Renderer::getCamera().near, 0.1f);
		ImGui::DragFloat("far", &Core::Renderer::getCamera().far, 0.1f);
		ImGui::TreePop();
	}
	if (ImGui::Button("Toggle AABB"))
	{
		Core::Renderer::toggleRenderAABB();
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

void processConsole()
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
