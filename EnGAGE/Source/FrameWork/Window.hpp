#pragma once
#include <cstdint>
#include <string>


struct GLFWwindow;
namespace FrameworkComponent {
	class Window {
	public:
		static void init(uint16_t screenWidth, uint16_t screenHeight, const std::string& titleName);
		static void destroy();
		static bool closeRequested();
		static void swapBuffers();
		static void pollEvents();


		inline static const uint16_t& getWidth() { return sScreenWidth; }
		inline static const uint16_t& getHeight() { return sScreenHeight; }
		inline static const bool& resized() { return sWindowResized; }
		inline static GLFWwindow* getRawWindow() { return sWindow; }
	private:
		static void setupCallbacks();
	private:
		static GLFWwindow* sWindow;
		static uint16_t sScreenWidth;
		static uint16_t sScreenHeight;
		static std::string sTitle;
		static bool sWindowResized;
	};
}