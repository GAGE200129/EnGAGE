#include "pch.hpp"
#include "Input.hpp"

#include "Core/LuaHostFunctions.hpp"

#include <GLFW/glfw3.h>

#include <imgui.h>

namespace Core::Input
{
	GLFWwindow* sWindow = nullptr;
	double sCursorX = 0.0f;
	double sCursorY = 0.0f;
	double sPrevCursorX = 0.0f;
	double sPrevCursorY = 0.0f;

	void init(GLFWwindow* rawWindow)
	{
		sWindow = rawWindow;

		glfwSetScrollCallback(rawWindow, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				if (yoffset > 0)
				{
					Messenger::recieveMessage(MessageType::SCROLL_UP);
				}
				else if (yoffset < 0)
				{
					Messenger::recieveMessage(MessageType::SCROLL_DOWN);
				}
			});
		glfwSetCursorPosCallback(rawWindow, [](GLFWwindow* window, double xpos, double ypos)
			{
				CursorMovedMessage cursorMessage;

				sCursorX = xpos;
				sCursorY = ypos;

				cursorMessage.x = (float)xpos;
				cursorMessage.y = (float)ypos;
				cursorMessage.dx = (float)(sCursorX - sPrevCursorX);
				cursorMessage.dy = (float)(sCursorY - sPrevCursorY);

				sPrevCursorX = sCursorX;
				sPrevCursorY = sCursorY;

				
				Messenger::recieveMessage(MessageType::CURSOR_MOVED , &cursorMessage);
			});

		glfwSetKeyCallback(rawWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				if (action == PRESS)
				{
					KeyPressedMessage message;
					message.keyCode = key;
					Messenger::recieveMessage(MessageType::KEY_PRESSED, &message);
				}
				else if (action == RELEASE)
				{
					KeyReleasedMessage message;
					message.keyCode = key;
					Messenger::recieveMessage(MessageType::KEY_RELEASED, &message);
				}
			});

		glfwSetMouseButtonCallback(rawWindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				if (action == PRESS)
				{
					ButtonPressedMessage message;
					message.buttonCode = button;
					Messenger::recieveMessage(MessageType::BUTTON_PRESSED, &message);
				}
				else if (action == RELEASE)
				{
					ButtonReleasedMessage message;
					message.buttonCode = button;
					Messenger::recieveMessage(MessageType::BUTTON_RELEASED, &message);
				}
			});

	}
	
	bool isButtonDown(Int32 button)
	{
		return glfwGetMouseButton(sWindow, button) == GLFW_PRESS;
	}

	const double& getX()
	{
		return sCursorX;
	}


	const double& getY()
	{
		return sCursorY;
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
	int luaToggleCursor(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 0);
		toggleCursor();
		return 0;
	}
	int luaIsCursorLocked(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 0);
		int result = cursorLocked();
		lua_pushboolean(L, result);
		return 1;
	}
}
