#pragma once

namespace Core
{
	namespace Renderer
	{
		struct Camera
		{
			float x, y, z;
			float pitch, yaw, roll;
			float fov, near, far;
		};

		void init();
		void shutdown();

		void render();

		Camera& getCamera();
	}
}