#pragma once

#include "MessageConstants.hpp"

namespace Core
{
	enum class MessageType : unsigned int
	{
		KEY_PRESSED,
		KEY_RELEASED,
		BUTTON_PRESSED,
		BUTTON_RELEASED,
		CURSOR_MOVED,
		TOGGLE_CURSOR,
		WINDOW_RESIZED,
		PHYSICS_UPDATE_RIGID_BODY,
		INIT_COLLISION_SHAPE,
		REMOVE_SCRIPT,
		COUNT
	};

	struct Message
	{
		MessageType type;
		unsigned char message[Messenger::BUFFER_SIZE];
	};

	const char* getMessageName(MessageType type);

	
}