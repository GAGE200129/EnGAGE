#pragma once

#include "Model.hpp"

namespace Core
{
	namespace Resource
	{
		void shutdown();
		Model* getModel(const String& filePath);
	}
}