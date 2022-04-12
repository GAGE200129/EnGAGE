#pragma once

#include "pch.hpp"

namespace Core
{
	namespace Scene
	{
		void loadScene(const String& filePath);
		void saveScene(const String& filePath);
		void checkForSceneSwitch();
		String& getLoadedSceneName();
	}
}