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
	private:
		void renderModel(const Model& pModel, const Node& node, glm::mat4x4 accumulatedTransform);
	public:
		Camera mCamera;
	private:
		TestShader mShader;
	};
}