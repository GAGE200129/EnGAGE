#pragma once



namespace Core
{	
	namespace GameEngine
	{
		void init(uint16_t width, uint16_t height, const String& title);
		void run();
		void clearResources();
	};
}