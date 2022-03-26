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


		bool isKeyPressed(uint16_t code);
		bool isKeyPressedOnce(uint16_t code);
		bool isKeyReleased(uint16_t code);
		bool isButtonPressed(uint16_t code);
		bool isButtonPressedOnce(uint16_t code);
		bool isButtonReleased(uint16_t code);

		double getCursorX();
		double getCursorY();
		double getCursorDX();
		double getCursorDY();

		void disableCursor();
		void enableCursor();
		void toggleCursor();
		bool cursorLocked();
	};
}
