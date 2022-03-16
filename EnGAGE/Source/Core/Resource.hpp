#pragma once

#include "Model.hpp"

namespace Core
{
	namespace Resource
	{
		const Model* getModel(const String& filePath);
		void cleanup();
	}
}