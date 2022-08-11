#include "pch.hpp"
#include "SceneGraphEditor.hpp"

#include "Core/ECS/ECS.hpp"
#include "Core/ECS/Transform.hpp"
#include "Core/Scene.hpp"

#include <ImGuizmo.h>
#include <misc/cpp/imgui_stdlib.h>


namespace Core::SceneGraphEditor
{
	static const ECS::EntitySignature* gSelectedEntity = nullptr;

	void processSceneGraph()
	{
		ImGui::Begin("SceneGraph");

		if (ImGui::BeginListBox("Entities"))
		{
			for (unsigned int i = 0; i < ECS::getEntityCount(); i++)
			{
				char buf[50];
				const auto& e = ECS::getEntitySignatures()[i];
				sprintf(buf, "Entity %llu", e.id);
				if (ImGui::Selectable(buf, gSelectedEntity == &e))
				{
					gSelectedEntity = &e;
				}
			}

			ImGui::EndListBox();
		}


		if (ImGui::Button("New"))
		{
			ECS::createEntity();
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete") && gSelectedEntity)
		{
			ECS::removeEntity(gSelectedEntity->id);
			gSelectedEntity = nullptr;
		}


		ImGui::End();
	}

	void processInspector()
	{
		using namespace ECS;
		ImGui::Begin("Inspector");
		//Process component
		if (gSelectedEntity) {
			for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
			{
				ComponentHeader* pHeader = (ComponentHeader*)getComponent(gSelectedEntity->id, (ComponentType)i);
				if (pHeader)
				{
					ComponentHint hint = getComponentHint(pHeader->type);
					ImGui::Text("%s", hint.name);
					pHeader->OnImGui(pHeader);

					ImGui::PushID(i);
					if (ImGui::Button("Remove"))
					{
						removeComponent(gSelectedEntity->id, (ComponentType)i);
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
						ECS::addComponent(gSelectedEntity->id, (ComponentType)i);
					}
				}
				ImGui::EndPopup();
			}

			//if (ImGui::Button("SavePreset"))
			//	ImGui::OpenPopup("SavePresetPopup");
			//if (ImGui::BeginPopup("SavePresetPopup"))
			//{
			//	static String filePath = "Resources/Presets/";
			//	ImGui::InputText("Location", &filePath);
			//
			//	if(ImGui::Button("Save"))
			//		Scene::savePreset(filePath, gSelectedEntity->id);
			//
			//	ImGui::EndPopup();
			//}
		}
		ImGui::End();
	}
}
