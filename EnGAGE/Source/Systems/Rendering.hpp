#pragma once

#include "Core/SystemBase.hpp"
#include "Core/Shader.hpp"

#include "Components/ModelRenderer.hpp"

namespace Core
{
	class Rendering : public SystemBase<ComponentType::MODEL_RENDERER, ModelRenderer>
	{
	public:
		Rendering();
		void render();
		void cleanup();
	private:
		Scope<Core::Shader> mShader;
	};
}