#pragma once


struct GLFWwindow;
namespace Core
{
	namespace Editor
	{
		void init(GLFWwindow* pWindow);
		void shutdown();

		void render();
	}
}