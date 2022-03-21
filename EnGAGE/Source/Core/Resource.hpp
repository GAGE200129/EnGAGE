#pragma once

#include "Model.hpp"

namespace Core
{
	namespace Resource
	{
		void init();
		Model* getModel(const String& filePath);
		const DynArr<Scope<Core::Model>>& getModels();
		void cleanup();
	}
}