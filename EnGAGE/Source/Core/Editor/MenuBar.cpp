#include "pch.hpp"
#include "MenuBar.hpp"

#include "Core/GameEngine.hpp"
#include "Core/Scene.hpp"

#include <misc/cpp/imgui_stdlib.h>

namespace Core::MenuBar
{
	void process()
	{
		static const char* scenePath = "Resources/Scenes";
		static std::string loadedscenePath = scenePath;
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
			ImGui::InputText("Path", &loadedscenePath);
			if (ImGui::Button("Save"))
			{
				Scene::saveScene(loadedscenePath);
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
					Scene::loadScene(pathName);
				}
			}
			ImGui::EndPopup();
		}
	}
}
