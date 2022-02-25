#pragma once


#include "Message.hpp"
#include "MessageMacro.hpp"

namespace Messaging
{
	class KeyPressed : public Message
	{
	public:
		KeyPressed(uint16_t keyCode) : mKeyCode(keyCode) {}

		EN_MESSAGE_TYPE(MessageType::KEY_PRESSED)
		EN_MESSAGE_CATEGORY(MessageCategory::INPUT)

		inline const uint16_t& getKeyCode() const { return mKeyCode; }
	private:
		uint16_t mKeyCode;
	};

	class KeyReleased : public Message
	{
	public:
		KeyReleased(uint16_t keyCode) : mKeyCode(keyCode) {}

		EN_MESSAGE_TYPE(MessageType::KEY_RELEASED)
		EN_MESSAGE_CATEGORY(MessageCategory::INPUT)

		inline const uint16_t& getKeyCode() const { return mKeyCode; }
	private:
		uint16_t mKeyCode;
	};
}