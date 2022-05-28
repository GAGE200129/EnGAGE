#pragma once

#include "MessageConstants.hpp"

struct lua_State;
namespace Core
{
	enum class MessageType : unsigned int
	{
		SCROLL_UP,
		SCROLL_DOWN,
		KEY_PRESSED,
		KEY_RELEASED,
		BUTTON_PRESSED,
		BUTTON_RELEASED,
		CURSOR_MOVED,
		WINDOW_RESIZED,
		COUNT
	};

	struct MessageData
	{
		const char* name;
		unsigned int size;
	};

	struct Message
	{
		MessageType type;
		Byte message[Messenger::BUFFER_SIZE];
	};

	struct KeyPressedMessage
	{
		unsigned int keyCode;
	};

	struct KeyReleasedMessage
	{
		unsigned int keyCode;
	};

	struct ButtonPressedMessage
	{
		unsigned int buttonCode;
	};

	struct ButtonReleasedMessage
	{
		unsigned int buttonCode;
	};

	struct CursorMovedMessage
	{
		float x, y, dx, dy;
	};

	struct WindowResizedMessage
	{
		unsigned int width, height;
	};


	const MessageData& getMessageData(MessageType type);
}