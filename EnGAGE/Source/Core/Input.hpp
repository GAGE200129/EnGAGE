#pragma once
#include "InputCodes.hpp"
#include "ButtonCodes.hpp"
#include "Core/Messenger/Messenger.hpp"

struct GLFWwindow;
namespace Core::Input
{
	static constexpr uint8_t RELEASE = 0;
	static constexpr uint8_t PRESS = 1;
	static constexpr uint8_t REPEAT = 2;

	void init(GLFWwindow* rawWindow);

	bool isButtonDown(Int32 button);

	const double& getX();
	const double& getY();
	void disableCursor();
	void enableCursor();
	void toggleCursor();
	bool cursorLocked();
}
