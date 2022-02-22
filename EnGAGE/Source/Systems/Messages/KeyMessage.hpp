#pragma once

#include <cstdint>

#include "Message.hpp"

namespace Systems
{
	class KeyPressed : public Message
	{
	public:
		KeyPressed(uint16_t keyCode) : mKeyCode(keyCode) {}

		MessageType getType() override { return MessageType::KEY_PRESSED; }

		inline const uint16_t& getKeyCode() const { return mKeyCode; }
	private:
		uint16_t mKeyCode;
	};

	class KeyReleased : public Message
	{
	public:
		KeyReleased(uint16_t keyCode) : mKeyCode(keyCode) {}

		MessageType getType() override { return MessageType::KEY_RELEASED; }

		inline const uint16_t& getKeyCode() const { return mKeyCode; }
	private:
		uint16_t mKeyCode;
	};
}