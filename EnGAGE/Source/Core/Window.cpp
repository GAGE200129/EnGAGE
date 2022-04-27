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
	GLFWwindow* sWindow = nullptr;
	int sFullScreenWidth = 0;
	int sFullScreenHeight = 0;
	int sWindowedScreenWidth = 0;
	int sWindowedScreenHeight = 0;
	std::string sTitle = "";
	bool sWindowResized = false;
	bool gFullScreen = false;

	static void setupCallbacks()
	{
		glfwSetFramebufferSizeCallback(sWindow, [](GLFWwindow* window, int width, int height) {
			sWindowResized = true;

			Message message;
			int data[] = { width, height };
			message.type = MessageType::WINDOW_RESIZED;
			memcpy(message.message, data, sizeof(data));
			Messenger::recieveMessage(&message);
		});
	}

	void init(int screenWidth, int screenHeight, int fullScreenWidth, int fullScreenHeight, const String& titleName)
	{
		sWindowedScreenWidth = screenWidth;
		sWindowedScreenHeight = screenHeight;
		sFullScreenWidth = fullScreenWidth;
		sFullScreenHeight = fullScreenHeight;
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

	}

	void destroy()
	{
		glfwTerminate();
	}

	void onMessage(const Message* pMessage)
	{
		if (pMessage->type == MessageType::KEY_PRESSED)
		{
			unsigned int keyCode = *reinterpret_cast<const unsigned int*>(pMessage->message);
			// Toggle full screen
			if (keyCode == InputCodes::KEY_F11) 
			{
				gFullScreen = !gFullScreen;

				if (gFullScreen)
				{
					Message message;
					int data[] = { sFullScreenWidth, sFullScreenHeight };
					message.type = MessageType::WINDOW_RESIZED;
					memcpy(message.message, data, sizeof(data));
					Messenger::queueMessage(&message);

					glfwSetWindowMonitor(sWindow, glfwGetPrimaryMonitor(), 0, 0, sFullScreenWidth, sFullScreenHeight, GLFW_DONT_CARE);
				}
				else
				{
					glfwSetWindowMonitor(sWindow, NULL, 0, 0, sWindowedScreenWidth, sWindowedScreenHeight, GLFW_DONT_CARE);
					const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
					glfwSetWindowPos(sWindow, int(mode->width / 2.0f - sWindowedScreenWidth / 2.0f), int(mode->height / 2.0f - sWindowedScreenHeight / 2.0f));
				}
			}
		}
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
	const bool& resized() { return sWindowResized; }
	GLFWwindow* getRawWindow() { return sWindow; }

}


