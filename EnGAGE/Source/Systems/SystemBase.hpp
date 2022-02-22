#pragma once

namespace Systems {
	class SystemBase 
	{
	public:
		virtual void handleMessage() = 0;
	};
}