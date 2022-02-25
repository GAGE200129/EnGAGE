#pragma once

#include <functional>

namespace FrameworkComponent
{
	class ButtonCodes {
	public:
		static constexpr uint16_t MOUSE_BUTTON_1 = 0;
		static constexpr uint16_t MOUSE_BUTTON_2 = 1;
		static constexpr uint16_t MOUSE_BUTTON_3 = 2;
		static constexpr uint16_t MOUSE_BUTTON_4 = 3;
		static constexpr uint16_t MOUSE_BUTTON_5 = 4;
		static constexpr uint16_t MOUSE_BUTTON_6 = 5;
		static constexpr uint16_t MOUSE_BUTTON_7 = 6;
		static constexpr uint16_t MOUSE_BUTTON_8 = 7;
		static constexpr uint16_t MOUSE_BUTTON_LAST = MOUSE_BUTTON_8;
		static constexpr uint16_t MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1;
		static constexpr uint16_t MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2;
		static constexpr uint16_t MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3;
		static constexpr uint16_t NUM_BUTTONS = 8;

	};
}