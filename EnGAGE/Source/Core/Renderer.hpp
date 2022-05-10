#pragma once

#include "Messenger.hpp"

namespace Core::Renderer
{
	struct Camera
	{
		float x, y, z;
		float pitch, yaw, roll;
		float fov, near, far;
	};

	void init(UInt32 currentWidth, UInt32 currentHeight);
	void onMessage(const Message* pMessage);
	void shutdown();
	void render();
	Camera& getCamera();
}