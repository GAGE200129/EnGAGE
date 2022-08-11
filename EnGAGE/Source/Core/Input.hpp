#pragma once
#include "InputCodes.hpp"
#include "ButtonCodes.hpp"
#include "Core/Messenger/Messenger.hpp"

namespace Core
{
	struct InputData
	{
		double cursorX = 0.0f;
		double cursorY = 0.0f;
		double prevCursorX = 0.0f;
		double prevCursorY = 0.0f;
	};

	namespace Input
	{
		static constexpr uint8_t RELEASE = 0;
		static constexpr uint8_t PRESS = 1;
		static constexpr uint8_t REPEAT = 2;

		void init();

		bool isButtonDown(Int32 button);

		void disableCursor();
		void enableCursor();
		void toggleCursor();
		bool cursorLocked();

		const InputData& getData();

		int luaToggleCursor(lua_State* L);
		int luaIsCursorLocked(lua_State* L);
	}
}
