#pragma once


namespace Core
{

	struct SceneData
	{
		bool sceneSwitch = false;
		std::string scenePath = "";
	};

	namespace Scene
	{
		void loadScene(const String& filePath);
		void saveScene(const String& filePath);
		void checkForSceneSwitch();
		const SceneData& getData();
	}
}