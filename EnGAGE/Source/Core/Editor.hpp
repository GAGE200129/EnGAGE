#pragma once

#include "Messaging.hpp"

struct GLFWwindow;
namespace Core
{
	namespace Editor
	{
		void init(GLFWwindow* pWindow);
		void shutdown();
		void onMessage(const Messaging::Message* pMessage);
		void render();
	}
}