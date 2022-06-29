#include "pch.hpp"
#include "GameEngineEditor.hpp"

#include "Core/Messenger/Messenger.hpp"

#include "Core/ECS/ECS.hpp"

namespace Core::GameEngineEditor
{
	void process()
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
}