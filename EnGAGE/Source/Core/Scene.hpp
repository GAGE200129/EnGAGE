#pragma once


namespace Core::Scene
{
	void loadPreset(const String& filePath);
	void savePreset(const String& filePath, UInt32 entityID);
	void loadScene(const String& filePath);
	void saveScene(const String& filePath);
	void checkForSceneSwitch();
	String& getLoadedSceneName();

	int luaDoPreset(lua_State* L);
}