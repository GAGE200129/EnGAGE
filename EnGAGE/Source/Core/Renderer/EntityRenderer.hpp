#pragma once

#include "Core/Camera.hpp"
#include "GBuffer.hpp"

namespace Core
{
	class EntityRenderer
	{
	public:
		void render(GBuffer& gBuffer, const Camera& camera, bool renderCullingSphere);
	private:
	};
}