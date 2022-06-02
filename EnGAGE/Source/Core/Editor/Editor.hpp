#pragma once

#include "Core/Messenger/Messenger.hpp"

struct GLFWwindow;
namespace Core::Editor
{
	void init(GLFWwindow* pWindow, UInt32 width, UInt32 height);
	void shutdown();
	void clear();
	void onMessage(const Message* pMessage);
	void update(F32 delta);
	void render();
	bool isEnabled();

	void toggle();
}