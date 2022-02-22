#include "Window.hpp"

#include <cassert>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

GLFWwindow* FrameworkComponent::Window::sWindow = nullptr;
uint16_t FrameworkComponent::Window::sScreenWidth = 0;
uint16_t FrameworkComponent::Window::sScreenHeight = 0;
std::string FrameworkComponent::Window::sTitle = "";
bool FrameworkComponent::Window::sWindowResized = false;

void FrameworkComponent::Window::init(uint16_t screenWidth, uint16_t screenHeight, const std::string& titleName)
{
	sScreenWidth = screenWidth;
	sScreenHeight = screenHeight;
	sTitle = titleName;

	assert(glfwInit() && "Failed to init glfw !");

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	sWindow = glfwCreateWindow(screenWidth, screenHeight, titleName.c_str(), NULL, NULL);
	if (!sWindow)
	{
		glfwTerminate();
		assert(!"Failed to create GLFW window !");
	}

	setupCallbacks();
}

void FrameworkComponent::Window::destroy()
{
	glfwTerminate();
}

bool FrameworkComponent::Window::closeRequested()
{
	return glfwWindowShouldClose(sWindow);
}

void FrameworkComponent::Window::swapBuffers()
{
	glfwSwapBuffers(sWindow);
}

void FrameworkComponent::Window::pollEvents()
{
	sWindowResized = false;
	glfwPollEvents();
}

void FrameworkComponent::Window::setupCallbacks()
{
	glfwSetFramebufferSizeCallback(sWindow, [](GLFWwindow* window, int width, int height) {
		sWindowResized = true;
		sScreenWidth = width;
		sScreenHeight = height;
	});
}
