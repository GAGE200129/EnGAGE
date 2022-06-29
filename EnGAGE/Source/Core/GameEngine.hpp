#pragma once

#include "Core/Data/Camera.hpp"

namespace Core
{
	struct GameEngineData
	{
		Camera mainCamera;
		Camera debugCamera;
		Camera* currentCamera = nullptr;
	};

	namespace GameEngine
	{
		void init(unsigned int width, unsigned int height, unsigned int fullScreenWidth, unsigned int fullScreenHeight, const String& title);
		void run();
		void clearResources();
		GameEngineData& getEngineData();

		int luaUpdateCameraPerspective(lua_State* L);
	}
}