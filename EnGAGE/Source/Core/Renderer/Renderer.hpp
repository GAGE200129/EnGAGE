#pragma once

#include "Core/Messenger.hpp"
#include "Core/Camera.hpp"

namespace Core::Renderer
{
	void init(UInt32 currentWidth, UInt32 currentHeight);
	void onMessage(const Message* pMessage);
	void shutdown();
	void render(const Camera& camera);
}