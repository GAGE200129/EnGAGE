#include "Input.hpp"

#include <GLFW/glfw3.h>

namespace FrameworkComponent
{
	bool Input::sKeys[InputCodes::NUM_KEYS];
	bool Input::sButtons[ButtonCodes::NUM_BUTTONS];
	double Input::sCursorX = 0.0f;
	double Input::sCursorY = 0.0f;
	std::function<void(int, int, int, int)> Input::sKeyFn = [](int, int, int, int) {};
	std::function<void(int, int, int)> Input::sButtonFn = [](int, int, int) {};
	std::function<void(double, double)> Input::sCursorPosFn = [](double, double) {};

	void Input::init(GLFWwindow* rawWindow)
	{
		//Clear sKeys arr
		for (int i = 0; i < InputCodes::NUM_KEYS; i++) {
			sKeys[i] = false;
		}

		//Clear sButtons arr
		for (int i = 0; i < ButtonCodes::NUM_BUTTONS; i++) {
			sButtons[i] = false;
		}

		glfwSetCursorPosCallback(rawWindow, [](GLFWwindow* window, double xpos, double ypos)
			{
				sCursorPosFn(xpos, ypos);
				sCursorX = xpos;
				sCursorY = ypos;
			});

		glfwSetKeyCallback(rawWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				sKeyFn(key, scancode, action, mods);
				if (action == PRESS)
				{
					sKeys[key] = true;
				}
				else if (action == RELEASE)
				{
					sKeys[key] = false;
				}
			});

		glfwSetMouseButtonCallback(rawWindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				sButtonFn(button, action, mods);
				if (action == PRESS)
				{
					sButtons[button] = true;
				}
				else if (action == RELEASE)
				{
					sButtons[button] = false;
				}
			});

	}
}
