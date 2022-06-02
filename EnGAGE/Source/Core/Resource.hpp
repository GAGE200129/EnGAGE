#pragma once

#include "Model.hpp"
#include "Map/TextureSheet.hpp"
namespace Core
{
	namespace Resource
	{
		void clear();
		const Model* getModel(const String& filePath);
		TextureSheet* getTextureSheet(const String& texture);
	}
}