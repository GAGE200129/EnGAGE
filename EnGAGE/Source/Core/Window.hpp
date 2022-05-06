#pragma once


#include "Messenger.hpp"

struct GLFWwindow;
namespace Core::Window {
	void init(unsigned int screenWidth, unsigned int screenHeight, unsigned int fullScreenWidth, unsigned int fullScreenHeight, const String& titleName);
	void destroy();
	void onMessage(const Message* pMessage);
	bool closeRequested();
	void swapBuffers();
	void pollEvents();
	double getCurrentTime();

	unsigned int getWidth();
	unsigned int getHeight();
	const char* getTitleName();
	const bool& resized();
	GLFWwindow* getRawWindow();	
}
