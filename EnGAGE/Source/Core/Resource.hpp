#pragma once

#include "Model.hpp"

namespace Core
{
	namespace Resource
	{
		void shutdown();
		const Model* getModel(const String& filePath);
	}
}