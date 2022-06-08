#pragma once

#include "Core/Data/Camera.hpp"

namespace Core::GameEngine
{
	void init(unsigned int width, unsigned int height, unsigned int fullScreenWidth, unsigned int fullScreenHeight, const String& title);
	void run();
	void clearResources();
	Camera& getMainCamera();
	Camera& getDebugCamera();
}