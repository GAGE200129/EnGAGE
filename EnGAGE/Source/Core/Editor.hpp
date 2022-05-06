#pragma once

#include "Messenger.hpp"

struct GLFWwindow;
namespace Core
{
	namespace Editor
	{
		void init(GLFWwindow* pWindow, int width, int height);
		void shutdown();
		void onMessage(const Message* pMessage);
		void render();
	}
}