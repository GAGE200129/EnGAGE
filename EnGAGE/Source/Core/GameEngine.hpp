#pragma once

#include "GameObject.hpp"

namespace Core
{
	namespace GameEngine
	{
		void init(uint16_t width, uint16_t height, const String& title);
		void run();

		void addGameObject(Ref<GameObject> go);
	};
}