#pragma once


#include "Messaging.hpp"

struct GLFWwindow;
namespace Core::Window {
	void init(int screenWidth, int screenHeight, int fullScreenWidth, int fullScreenHeight, const String& titleName);
	void destroy();
	void onMessage(const Messaging::Message* pMessage);
	bool closeRequested();
	void swapBuffers();
	void pollEvents();
	double getCurrentTime();

	const bool& resized();
	GLFWwindow* getRawWindow();	
}
