#pragma once


namespace Core::Scene
{
	void loadScene(const String& filePath);
	void saveScene(const String& filePath);
	void checkForSceneSwitch();
	String& getLoadedSceneName();
}