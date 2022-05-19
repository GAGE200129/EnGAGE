#pragma once

#include "Model.hpp"

namespace Core
{
	namespace Resource
	{
		void clear();
		const Model* getModel(const String& filePath);
	}
}