#pragma once

#include "Core/SystemBase.hpp"
#include "TestShader.hpp"

#include "Components/ModelRenderer.hpp"

namespace Core
{
	struct Camera
	{
		float x, y, z;// Position data
		float pitch, yaw, roll;

		float fov, near, far;
	};
	class Rendering : public SystemBase<ComponentType::MODEL_RENDERER, ModelRenderer>
	{
	public:
		Rendering();
		void render();
		void cleanup();
	public:
		Camera mCamera;
	private:
		TestShader mShader;
	};
}