#pragma once


#include "Core/Messenger/Messenger.hpp"

struct GLFWwindow;
namespace Core {

	struct WindowData
	{
		GLFWwindow* window = nullptr;
		unsigned int screenWidth = 0;
		unsigned int screenHeight = 0;
		unsigned int fullScreenWidth = 0;
		unsigned int fullScreenHeight = 0;
		unsigned int windowedScreenWidth = 0;
		unsigned int windowedScreenHeight = 0;
		std::string title = "";
		bool windowResized = false;
		bool fullScreen = false;
	};

	namespace Window
	{
		void init(unsigned int screenWidth, unsigned int screenHeight, unsigned int fullScreenWidth, unsigned int fullScreenHeight, const String& titleName);
		void destroy();
		bool closeRequested();
		void swapBuffers();
		void pollEvents();
		double getCurrentTime();

		void toggleFullScreen();

		const WindowData& getData();
	}
}
