#include "pch.hpp"
#include "Window.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Core
{
	namespace Window
	{
		GLFWwindow* sWindow = nullptr;
		uint16_t sScreenWidth = 0;
		uint16_t sScreenHeight = 0;
		std::string sTitle = "";
		bool sWindowResized = false;

		static void setupCallbacks()
		{
			glfwSetFramebufferSizeCallback(sWindow, [](GLFWwindow* window, int width, int height) {
				sWindowResized = true;
				sScreenWidth = width;
				sScreenHeight = height;
				});
		}

		void init(uint16_t screenWidth, uint16_t screenHeight, const std::string& titleName)
		{
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
			glfwSwapBuffers(sWindow);
		}

		void pollEvents()
		{
			sWindowResized = false;
			glfwPollEvents();
		}

		double getCurrentTime()
		{
			return glfwGetTime();
		}

		const uint16_t& getWidth() { return sScreenWidth; }
		const uint16_t& getHeight() { return sScreenHeight; }
		const bool& resized() { return sWindowResized; }
		GLFWwindow* getRawWindow() { return sWindow; }
	}
}


