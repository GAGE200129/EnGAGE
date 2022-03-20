#pragma once

#include "Model.hpp"

namespace Core
{
	namespace Resource
	{
		void init();
		Ptr<Model> getModel(const String& filePath);
		const DynArr<Ref<Core::Model>>& getModels();
		void cleanup();
	}
}