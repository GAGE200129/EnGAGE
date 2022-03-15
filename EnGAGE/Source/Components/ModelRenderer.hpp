#pragma once

#include "Core/ComponentBase.hpp"

#include "Core/Model.hpp"

namespace Core
{
	class ModelRenderer : public ComponentBase
	{
	public:
		ModelRenderer(Ptr<Model> model) : ComponentBase(ComponentType::MODEL_RENDERER), mModel(model) {}

	public:
		Ptr<Model> mModel;
	};
}