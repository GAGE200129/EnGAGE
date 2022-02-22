#pragma once

namespace Systems
{
	enum class MessageType {
		KEY_PRESSED,
		KEY_RELEASED,
		BUTTON_PRESSED,
		BUTTON_RELEASED,
	};
	class Message 
	{
	public:
		virtual MessageType getType() = 0;
	};
}