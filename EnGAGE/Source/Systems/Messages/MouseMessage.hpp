#pragma once

#include "Message.hpp"
#include "MessageMacro.hpp"

namespace Messaging
{

	class ToggleMouse : public Message {
	public:
		EN_MESSAGE_TYPE(MessageType::MOUSE_TOGGLE)
		EN_MESSAGE_CATEGORY(MessageCategory::INPUT)
	};

	class MouseDisable : public Message {
	public:
		EN_MESSAGE_TYPE(MessageType::MOUSE_DISABLE)
		EN_MESSAGE_CATEGORY(MessageCategory::INPUT)
	};

	class MouseEnable : public Message {
	public:
		EN_MESSAGE_TYPE(MessageType::MOUSE_ENABLE)
		EN_MESSAGE_CATEGORY(MessageCategory::INPUT)
	};

	class MouseButtonPressed : public Message {
	public:
		MouseButtonPressed(uint16_t keyCode) : mButtonCode(keyCode) {}

		EN_MESSAGE_TYPE(MessageType::BUTTON_PRESSED)
		EN_MESSAGE_CATEGORY(MessageCategory::INPUT)

		inline const uint16_t& getButtonCode() const { return mButtonCode; }
	private:
		uint16_t mButtonCode;
	};

	class MouseButtonReleased : public Message {
	public:
		MouseButtonReleased(uint16_t keyCode) : mButtonCode(keyCode) {}

		EN_MESSAGE_TYPE(MessageType::BUTTON_RELEASED)
		EN_MESSAGE_CATEGORY(MessageCategory::INPUT)

		inline const uint16_t& getButtonCode() const { return mButtonCode; }
	private:
		uint16_t mButtonCode;
	};

	class MouseMoved : public Message {
	public:
		MouseMoved(double x, double y) : mX(x), mY(y) {}

		EN_MESSAGE_TYPE(MessageType::MOUSE_MOVED)
		EN_MESSAGE_CATEGORY(MessageCategory::INPUT)

		inline const double& getX() const { return mX; }
		inline const double& getY() const { return mY; }
	private:
		double mX, mY;
	};
}