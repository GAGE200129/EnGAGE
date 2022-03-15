#pragma once
#include "InputCodes.hpp"
#include "ButtonCodes.hpp"

struct GLFWwindow;
namespace Core
{
	namespace Input
	{
		static constexpr uint8_t RELEASE = 0;
		static constexpr uint8_t PRESS = 1;
		static constexpr uint8_t REPEAT = 2;

		void init(GLFWwindow* rawWindow);
		void update();

		void setKeyfn(const std::function<void()>& keyFn);
		void setButtonfn(const std::function<void()>& buttonFn);
		void setCursorPosFn(const std::function<void()>& cursorPosFn);

		bool isKeyPressed(uint16_t code);
		bool isKeyPressedOnce(uint16_t code);
		bool isKeyReleased(uint16_t code);
		bool isButtonPressed(uint16_t code);
		bool isButtonPressedOnce(uint16_t code);
		bool isButtonReleased(uint16_t code);

		double getCursorX();
		double getCursorY();

		void disableCursor();
		void enableCursor();
		void toggleCursor();
	};
}
