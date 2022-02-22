#pragma once

#include <functional>

#include "InputCodes.hpp"
#include "ButtonCodes.hpp"

struct GLFWwindow;
namespace FrameworkComponent
{

	class Input
	{
	public:
		static constexpr uint8_t RELEASE = 0;
		static constexpr uint8_t PRESS = 1;
		static constexpr uint8_t REPEAT = 2;
	public:
		static void init(GLFWwindow* rawWindow);

		static void setKeyfn(const std::function<void(int, int, int, int)>& keyFn) {
			sKeyFn = keyFn;
		}

		static void setButtonfn(const std::function<void(int, int, int)>& buttonFn) {
			sButtonFn = buttonFn;
		}

		static void setCursorPosFn(const std::function<void(double, double)>& cursorPosFn) {
			sCursorPosFn = cursorPosFn;
		}

	private:
		static bool sKeys[InputCodes::NUM_KEYS];
		static bool sButtons[ButtonCodes::NUM_BUTTONS];
		static double sCursorX, sCursorY;
		static std::function<void(int, int, int, int)> sKeyFn;
		static std::function<void(int, int, int)> sButtonFn;
		static std::function<void(double, double)> sCursorPosFn;
	};
}