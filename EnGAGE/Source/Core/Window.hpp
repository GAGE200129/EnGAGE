#pragma once

struct GLFWwindow;
namespace Core {
	namespace Window {
		void init(uint16_t screenWidth, uint16_t screenHeight, const std::string& titleName);
		void destroy();
		bool closeRequested();
		void swapBuffers();
		void pollEvents();
		double getCurrentTime();

		const uint16_t& getWidth();
		const uint16_t& getHeight();
		const bool& resized();
		GLFWwindow* getRawWindow();
		
	};
}
