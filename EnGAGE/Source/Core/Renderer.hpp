#pragma once

#include "Messaging.hpp"

namespace Core::Renderer
{
	struct Camera
	{
		float x, y, z;
		float pitch, yaw, roll;
		float fov, near, far;
	};

	void init(unsigned int currentWidth, unsigned int currentHeight);
	void onMessage(const Messaging::Message* pMessage);
	void shutdown();
	void render();
	Camera& getCamera();
}