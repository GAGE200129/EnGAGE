#pragma once

namespace Messaging
{
	enum class MessageType {
		KEY_PRESSED,
		KEY_RELEASED,
		BUTTON_PRESSED,
		BUTTON_RELEASED,
		MOUSE_MOVED,
		MOUSE_DISABLE,
		MOUSE_ENABLE,
		MOUSE_TOGGLE,
	};
}