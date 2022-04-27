#include "pch.hpp"
#include "Input.hpp"

#include "Messenger.hpp"

#include <GLFW/glfw3.h>

#include <imgui.h>

namespace Core::Input
{
	GLFWwindow* sWindow = nullptr;
	bool sKeys[InputCodes::NUM_KEYS];
	bool sPrevKeys[InputCodes::NUM_KEYS];
	bool sButtons[ButtonCodes::NUM_BUTTONS];
	bool sPrevButtons[ButtonCodes::NUM_BUTTONS];
	double sCursorX = 0.0f;
	double sCursorY = 0.0f;
	double sPrevCursorX = 0.0f;
	double sPrevCursorY = 0.0f;

	void init(GLFWwindow* rawWindow)
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
				sCursorX = xpos;
				sCursorY = ypos;
				float data[] = { (float)xpos, (float)ypos, (float)(sCursorX - sPrevCursorX), (float)(sCursorY - sPrevCursorY) };

				sPrevCursorX = sCursorX;
				sPrevCursorY = sCursorY;

				Message message = { MessageType::CURSOR_MOVED };
				memcpy(message.message, data, sizeof(data));
				Messenger::recieveMessage(&message);
			});

		glfwSetKeyCallback(rawWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				if (action == PRESS)
				{
					Message message = { MessageType::KEY_PRESSED };
					memcpy(message.message, &key, sizeof(int));
					Messenger::recieveMessage(&message);
					sKeys[key] = true;
				}
				else if (action == RELEASE)
				{
					Message message = { MessageType::KEY_RELEASED };
					memcpy(message.message, &key, sizeof(int));
					Messenger::recieveMessage(&message);

					sKeys[key] = false;
				}
			});

		glfwSetMouseButtonCallback(rawWindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				if (action == PRESS)
				{
					Message message = { MessageType::BUTTON_PRESSED };
					memcpy(message.message, &button, sizeof(int));
					Messenger::recieveMessage(&message);
					sButtons[button] = true;
				}
				else if (action == RELEASE)
				{
					Message message = { MessageType::BUTTON_RELEASED };
					memcpy(message.message, &button, sizeof(int));
					Messenger::recieveMessage(&message);

					sButtons[button] = false;
				}
			});

	}
	void onMessage(const Message* pMessage)
	{
		switch (pMessage->type)
		{
		case MessageType::TOGGLE_CURSOR:
		{
			toggleCursor();
			break;
		}
		}
	}
	void onRequest(Request* pRequest)
	{
		switch (pRequest->type)
		{
		case RequestType::CURSOR_POS:
		{
			double data[] = { (float)getCursorX(), (float)getCursorY() };
			memcpy(pRequest->data, data, sizeof(data));
			break;
		}
		case RequestType::CURSOR_DELTA:
		{
			double data[] = { (float)getCursorDX(), (float)getCursorDY() };
			memcpy(pRequest->data, data, sizeof(data));
			break;
		}
		case RequestType::CURSOR_LOCKED:
		{
			int data = cursorLocked();
			memcpy(pRequest->data, &data, sizeof(data));
			break;
		}
		}
	}
	void update()
	{
		
		memcpy(sPrevKeys, sKeys, sizeof(sKeys));
		memcpy(sPrevButtons, sButtons, sizeof(sButtons));
	}


	bool  isKeyPressed(uint16_t code) { return sKeys[code]; }
	bool  isKeyPressedOnce(uint16_t code) { return sKeys[code] && !sPrevKeys[code]; }
	bool  isKeyReleased(uint16_t code) { return !sKeys[code]; }
	bool  isButtonPressed(uint16_t code) { return sButtons[code]; }
	bool  isButtonPressedOnce(uint16_t code) { return sButtons[code] && !sPrevButtons[code]; }
	bool  isButtonReleased(uint16_t code) { return !sButtons[code]; }

	double  getCursorX() { return sCursorX; }
	double  getCursorY() { return sCursorY; }

	double  getCursorDX()
	{
		return sCursorX - sPrevCursorX;
	}

	double  getCursorDY()
	{
		return sCursorY - sPrevCursorY;
	}

	void  disableCursor()
	{
		glfwSetInputMode(sWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(sWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	void enableCursor()
	{
		glfwSetInputMode(sWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(sWindow, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
	}
	void  toggleCursor()
	{
		int status = glfwGetInputMode(sWindow, GLFW_CURSOR);
		if (status == GLFW_CURSOR_DISABLED) {
			enableCursor();
		}
		else if (status == GLFW_CURSOR_NORMAL) {
			disableCursor();
		}
	}
	bool  cursorLocked()
	{
		int status = glfwGetInputMode(sWindow, GLFW_CURSOR);
		if (status == GLFW_CURSOR_DISABLED) {
			return true;
		}
		else if (status == GLFW_CURSOR_NORMAL) {
			return false;
		}
		return false;
	}
}
