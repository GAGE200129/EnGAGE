#include "pch.hpp"
#include "MessageTypes.hpp"

#define TO_STRING(x) case MessageType::x: return #x

const char* Core::getMessageName(MessageType type)
{
	switch (type)
	{
		TO_STRING(KEY_PRESSED);
		TO_STRING(KEY_RELEASED);
		TO_STRING(BUTTON_PRESSED);
		TO_STRING(BUTTON_RELEASED);
		TO_STRING(CURSOR_MOVED);
		TO_STRING(TOGGLE_CURSOR);
		TO_STRING(WINDOW_RESIZED);
		TO_STRING(PHYSICS_UPDATE_RIGID_BODY);
		TO_STRING(INIT_COLLISION_SHAPE);
		TO_STRING(REMOVE_SCRIPT);
	}
	EN_ASSERT(false, "Unknown message type: {}", (unsigned int)type);
	return "";
}
