#pragma once

#include "ECSConstants.hpp"
#include "ComponentHeader.hpp"
#include "ComponentTypes.hpp"
#include "ComponentHint.hpp"


namespace Core
{
	
	struct ComponentArray
	{
		UInt64 count;
		UInt64 size;
		Scope<Byte[]> data;
		OrderedMap<UInt64, UInt64> entityToIndex;
	};

	

	const ComponentHint getComponentHint(ComponentType type);
	
	void initComponent(ComponentHeader* pHeader, ComponentType type);
}
