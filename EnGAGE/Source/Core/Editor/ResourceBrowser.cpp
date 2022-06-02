#include "pch.hpp"
#include "ResourceBrowser.hpp"

namespace Core::ResourceBrowser
{
	void process()
	{
		static const char* modelPath = "Resources/Models";
		static const char* scriptPath = "Resources/Scripts";
		static const char* texturePath = "Resources/TextureSheets";
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

		if (ImGui::TreeNode("TextureSheets"))
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(texturePath))
			{
				std::filesystem::path path = entry.path();
				String pathName = path.make_preferred().string();
				std::replace(pathName.begin(), pathName.end(), '\\', '/');

				ImGui::Selectable(pathName.c_str());
				if (ImGui::BeginDragDropSource())
				{
					ImGui::Text(pathName.c_str());
					ImGui::SetDragDropPayload("TEXTURE_SHEET", pathName.c_str(), pathName.size() + 1);
					ImGui::EndDragDropSource();
				}
			}

			ImGui::TreePop();
		}

		ImGui::End();
	}
}
