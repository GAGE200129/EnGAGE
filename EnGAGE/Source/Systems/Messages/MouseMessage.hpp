#pragma once
#include <cstdint>

#include "Message.hpp"

namespace Systems
{
	class MouseButtonPressed : public Message {
	public:
		MouseButtonPressed(uint16_t keyCode) : mButtonCode(keyCode) {}

		MessageType getType() override { return MessageType::BUTTON_PRESSED; }

		inline const uint16_t& getButtonCode() const { return mButtonCode; }
	private:
		uint16_t mButtonCode;
	};

	class MouseButtonReleased : public Message {
	public:
		MouseButtonReleased(uint16_t keyCode) : mButtonCode(keyCode) {}

		MessageType getType() override { return MessageType::BUTTON_RELEASED; }

		inline const uint16_t& getButtonCode() const { return mButtonCode; }
	private:
		uint16_t mButtonCode;
	};
}