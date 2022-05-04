#pragma once


#include "Messenger.hpp"

struct GLFWwindow;
namespace Core::Window {
	void init(int screenWidth, int screenHeight, int fullScreenWidth, int fullScreenHeight, const String& titleName);
	void destroy();
	void onMessage(const Message* pMessage);
	bool closeRequested();
	void swapBuffers();
	void pollEvents();
	double getCurrentTime();

	const char* getTitleName();
	const bool& resized();
	GLFWwindow* getRawWindow();	
}
