#pragma once

#include "Core/ComponentBase.hpp"

#include "Core/Model.hpp"

namespace Core
{
	class ModelRenderer : public ComponentBase
	{
	public:
		ModelRenderer(const Model* model) : ComponentBase(ComponentType::MODEL_RENDERER), pModel(model) {}

	public:
		const Model* pModel;
	};
}