#include "pch.hpp"
#include "Window.hpp"

#include "InputCodes.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace Core::Window
{
	static GLFWwindow* sWindow = nullptr;
	static unsigned int sScreenWidth = 0;
	static unsigned int sScreenHeight = 0;
	static unsigned int sFullScreenWidth = 0;
	static unsigned int sFullScreenHeight = 0;
	static unsigned int sWindowedScreenWidth = 0;
	static unsigned int sWindowedScreenHeight = 0;
	static std::string sTitle = "";
	static bool sWindowResized = false;
	static bool gFullScreen = false;

	static void setupCallbacks()
	{
		glfwSetFramebufferSizeCallback(sWindow, [](GLFWwindow* window, int width, int height) {
			sWindowResized = true;
			sScreenWidth = width;
			sScreenHeight = height;
			Message message;
			int data[] = { width, height };
			message.type = MessageType::WINDOW_RESIZED;
			memcpy(message.message, data, sizeof(data));
			Messenger::recieveMessage(&message);
			});
	}

	void init(unsigned int screenWidth, unsigned int screenHeight, unsigned int fullScreenWidth, unsigned int fullScreenHeight, const String& titleName)
	{
		sWindowedScreenWidth = screenWidth;
		sWindowedScreenHeight = screenHeight;
		sFullScreenWidth = fullScreenWidth;
		sFullScreenHeight = fullScreenHeight;
		sScreenWidth = screenWidth;
		sScreenHeight = screenHeight;
		sTitle = titleName;

		EN_ASSERT(glfwInit(), "Failed to init glfw !");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		sWindow = glfwCreateWindow(screenWidth, screenHeight, titleName.c_str(), NULL, NULL);
		if (!sWindow)
		{
			const char* errorMessage = nullptr;
			glfwGetError(&errorMessage);
			EN_ASSERT(false, "Failed to create GLFW window: {}", errorMessage);
			glfwTerminate();
		}
		glfwMakeContextCurrent(sWindow);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		setupCallbacks();
		glfwSwapInterval(1);
	}

	void destroy()
	{
		glfwTerminate();
	}

	bool closeRequested()
	{
		return glfwWindowShouldClose(sWindow);
	}

	void swapBuffers()
	{
		sWindowResized = false;
		glfwSwapBuffers(sWindow);
	}

	void pollEvents()
	{
		glfwPollEvents();
	}

	double getCurrentTime()
	{
		return glfwGetTime();
	}
	void toggleFullScreen()
	{
		//Toggle fullscreen
		gFullScreen = !gFullScreen;

		if (gFullScreen)
		{
			WindowResizedMessage message;
			message.width = sFullScreenWidth;
			message.height = sFullScreenHeight;
			Messenger::queueMessage(MessageType::WINDOW_RESIZED, &message);

			glfwSetWindowMonitor(sWindow, glfwGetPrimaryMonitor(), 0, 0, sFullScreenWidth, sFullScreenHeight, GLFW_DONT_CARE);
		}
		else
		{
			glfwSetWindowMonitor(sWindow, NULL, 0, 0, sWindowedScreenWidth, sWindowedScreenHeight, GLFW_DONT_CARE);
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowPos(sWindow, int(mode->width / 2.0f - sWindowedScreenWidth / 2.0f), int(mode->height / 2.0f - sWindowedScreenHeight / 2.0f));
		}
	}
	unsigned int getWidth()
	{
		return sScreenWidth;
	}
	unsigned int getHeight()
	{
		return sScreenHeight;
	}
	const char* getTitleName()
	{
		return sTitle.c_str();
	}
	const bool& resized() { return sWindowResized; }
	GLFWwindow* getRawWindow() { return sWindow; }

}


