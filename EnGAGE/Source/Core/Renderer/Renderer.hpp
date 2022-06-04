#pragma once

#include "Core/Messenger/Messenger.hpp"
#include "Core/Camera.hpp"
#include "GBuffer.hpp"

namespace Core::Renderer
{
	void init(UInt32 currentWidth, UInt32 currentHeight);
	void onMessage(const Message* pMessage);
	void shutdown();
	void render(const Camera& camera);

	bool& isRenderCullingSphere();
	void setRenderScale(F32 scale);

	GBuffer& getGBuffer();
}