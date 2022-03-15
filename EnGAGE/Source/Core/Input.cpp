#include "pch.hpp"
#include "Input.hpp"

#include <GLFW/glfw3.h>

namespace Core
{
	GLFWwindow* sWindow = nullptr;
	bool sKeys[InputCodes::NUM_KEYS];
	bool sPrevKeys[InputCodes::NUM_KEYS];
	bool sButtons[ButtonCodes::NUM_BUTTONS];
	bool sPrevButtons[ButtonCodes::NUM_BUTTONS];
	double sCursorX = 0.0f;
	double sCursorY = 0.0f;
	FunctionPtr<void()> sKeyFn = []() {};
	FunctionPtr<void()> sButtonFn = []() {};
	FunctionPtr<void()> sCursorPosFn = []() {};

	void Input::init(GLFWwindow* rawWindow)
	{
		sWindow = rawWindow;
		//Clear sKeys arr
		for (int i = 0; i < InputCodes::NUM_KEYS; i++) {
			sKeys[i] = false;
			sPrevKeys[i] = false;
		}

		//Clear sButtons arr
		for (int i = 0; i < ButtonCodes::NUM_BUTTONS; i++) {
			sButtons[i] = false;
			sPrevButtons[i] = false;
		}

		glfwSetCursorPosCallback(rawWindow, [](GLFWwindow* window, double xpos, double ypos)
			{
				sCursorPosFn();
				sCursorX = xpos;
				sCursorY = ypos;
			});

		glfwSetKeyCallback(rawWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				sKeyFn();
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
				sButtonFn();
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
	void Input::update()
	{
		size_t keyArrSize = sizeof(sKeys);
		size_t buttonArrSize = sizeof(sButtons);
		memcpy_s(sPrevKeys, keyArrSize, sKeys, keyArrSize);
		memcpy_s(sPrevButtons, buttonArrSize, sButtons, buttonArrSize);
	}
	void Input::setKeyfn(const std::function<void()>& keyFn)
	{
		sKeyFn = keyFn;
	}

	void Input::setButtonfn(const std::function<void()>& buttonFn)
	{
		sButtonFn = buttonFn;
	}

	void Input::setCursorPosFn(const std::function<void()>& cursorPosFn)
	{
		sCursorPosFn = cursorPosFn;
	}

	bool Input::isKeyPressed(uint16_t code) { return sKeys[code]; }
	bool Input::isKeyPressedOnce(uint16_t code) { return sKeys[code] && !sPrevKeys[code]; }
	bool Input::isKeyReleased(uint16_t code) { return !sKeys[code]; }
	bool Input::isButtonPressed(uint16_t code) { return sButtons[code]; }
	bool Input::isButtonPressedOnce(uint16_t code) { return sButtons[code] && !sPrevButtons[code]; }
	bool Input::isButtonReleased(uint16_t code) { return !sButtons[code]; }

	double Input::getCursorX() { return sCursorX; }
	double Input::getCursorY() { return sCursorY; }

	void Input::disableCursor()
	{
		glfwSetInputMode(sWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(sWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	void Input::enableCursor()
	{
		glfwSetInputMode(sWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(sWindow, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
	}
	void Input::toggleCursor()
	{
		int status = glfwGetInputMode(sWindow, GLFW_CURSOR);
		if (status == GLFW_CURSOR_DISABLED) {
			enableCursor();
		}
		else if (status == GLFW_CURSOR_NORMAL) {
			disableCursor();
		}
	}
}
