#pragma once

#include "ECSConstants.hpp"
#include "Components/ComponentHeader.hpp"
#include "Components/ComponentTypes.hpp"
#include "Components/ComponentHint.hpp"


namespace Core
{
	
	struct ComponentArray
	{
		UInt64 count;
		UInt64 size;
		Scope<Byte[]> data;
		Map<UInt64, UInt64> entityToIndex;
	};

	

	const ComponentHint getComponentHint(ComponentType type);
	
	void initComponent(ComponentHeader* pHeader, ComponentType type);
}
