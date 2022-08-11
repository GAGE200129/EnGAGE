#include "pch.hpp"
#include "Input.hpp"

#include "Core/LuaHostFunctions.hpp"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <Core/Window.hpp>

namespace Core::Input
{
	static InputData gData;

	void init()
	{
		const WindowData& data = Window::getData();
		glfwSetScrollCallback(data.window, [](GLFWwindow* window, double xoffset, double yoffset)
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
		glfwSetCursorPosCallback(data.window, [](GLFWwindow* window, double xpos, double ypos)
			{
				CursorMovedMessage cursorMessage;

				gData.cursorX = xpos;
				gData.cursorY = ypos;

				cursorMessage.x = (float)xpos;
				cursorMessage.y = (float)ypos;
				cursorMessage.dx = (float)(gData.cursorX - gData.prevCursorX);
				cursorMessage.dy = (float)(gData.cursorY - gData.prevCursorY);

				gData.prevCursorX = gData.cursorX;
				gData.prevCursorY = gData.cursorY;

				
				Messenger::recieveMessage(MessageType::CURSOR_MOVED , &cursorMessage);
			});

		glfwSetKeyCallback(data.window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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

		glfwSetMouseButtonCallback(data.window, [](GLFWwindow* window, int button, int action, int mods)
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
		const WindowData& data = Window::getData();
		return glfwGetMouseButton(data.window, button) == GLFW_PRESS;
	}

	void  disableCursor()
	{
		const WindowData& data = Window::getData();
		glfwSetInputMode(data.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(data.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	void enableCursor()
	{
		const WindowData& data = Window::getData();
		glfwSetInputMode(data.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(data.window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
	}
	void  toggleCursor()
	{
		const WindowData& data = Window::getData();
		int status = glfwGetInputMode(data.window, GLFW_CURSOR);
		if (status == GLFW_CURSOR_DISABLED) {
			enableCursor();
		}
		else if (status == GLFW_CURSOR_NORMAL) {
			disableCursor();
		}
	}
	bool  cursorLocked()
	{
		const WindowData& data = Window::getData();
		int status = glfwGetInputMode(data.window, GLFW_CURSOR);
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
	const InputData& getData()
	{
		return gData;
	}
}
