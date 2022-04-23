#pragma once



namespace Core
{	
	namespace GameEngine
	{
		void init(unsigned int width, unsigned int height, unsigned int fullScreenWidth, unsigned int fullScreenHeight, const String& title);
		void run();
		void clearResources();
	};
}