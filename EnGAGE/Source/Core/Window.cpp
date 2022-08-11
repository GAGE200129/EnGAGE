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
	static WindowData gData;

	static void setupCallbacks()
	{
		glfwSetFramebufferSizeCallback(gData.window, [](GLFWwindow* window, int width, int height) {
			gData.windowResized = true;
			gData.screenWidth = width;
			gData.screenHeight = height;
			Message message;
			int data[] = { width, height };
			message.type = MessageType::WINDOW_RESIZED;
			memcpy(message.message, data, sizeof(data));
			Messenger::recieveMessage(&message);
			});
	}

	void init(unsigned int screenWidth, unsigned int screenHeight, unsigned int fullScreenWidth, unsigned int fullScreenHeight, const String& titleName)
	{
		gData.windowedScreenWidth = screenWidth;
		gData.windowedScreenHeight = screenHeight;
		gData.fullScreenWidth = fullScreenWidth;
		gData.fullScreenHeight = fullScreenHeight;
		gData.screenWidth = screenWidth;
		gData.screenHeight = screenHeight;
		gData.title = titleName;

		EN_ASSERT(glfwInit(), "Failed to init glfw !");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		gData.window = glfwCreateWindow(screenWidth, screenHeight, titleName.c_str(), NULL, NULL);
		if (!gData.window)
		{
			const char* errorMessage = nullptr;
			glfwGetError(&errorMessage);
			EN_ASSERT(false, "Failed to create GLFW window: {}", errorMessage);
			glfwTerminate();
		}
		glfwMakeContextCurrent(gData.window);
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
		return glfwWindowShouldClose(gData.window);
	}

	void swapBuffers()
	{
		gData.windowResized = false;
		glfwSwapBuffers(gData.window);
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
		gData.fullScreen = !gData.fullScreen;

		if (gData.fullScreen)
		{
			WindowResizedMessage message;
			message.width = gData.fullScreenWidth;
			message.height = gData.fullScreenHeight;
			Messenger::queueMessage(MessageType::WINDOW_RESIZED, &message);

			gData.screenWidth = gData.fullScreenWidth;
			gData.screenHeight  = gData.fullScreenHeight;
			glfwSetWindowMonitor(gData.window, glfwGetPrimaryMonitor(), 0, 0, gData.fullScreenWidth, gData.fullScreenHeight, GLFW_DONT_CARE);
		}
		else
		{
			WindowResizedMessage message;
			message.width = gData.windowedScreenWidth;
			message.height = gData.windowedScreenWidth;
			Messenger::queueMessage(MessageType::WINDOW_RESIZED, &message);
			gData.screenWidth = gData.windowedScreenWidth;
			gData.screenHeight = gData.windowedScreenHeight;
			glfwSetWindowMonitor(gData.window, NULL, 0, 0, gData.windowedScreenWidth, gData.windowedScreenHeight, GLFW_DONT_CARE);
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowPos(gData.window, int(mode->width / 2.0f - gData.windowedScreenWidth / 2.0f), int(mode->height / 2.0f - gData.windowedScreenHeight / 2.0f));
		}
	}
	const WindowData& getData()
	{
		return gData;
	}


}


