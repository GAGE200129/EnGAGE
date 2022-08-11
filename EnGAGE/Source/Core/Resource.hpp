#pragma once

#include "Core/Data/Model.hpp"
#include "Map/TextureSheet.hpp"
namespace Core
{
	namespace Resource
	{
		void clear();
		Model* getModel(const String& filePath);
		TextureSheet* getTextureSheet(const String& texture);
	}
}