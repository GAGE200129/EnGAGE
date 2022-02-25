#pragma once



#include "Messages/Message.hpp"

namespace Systems {
	class SystemBase 
	{
	public:
		virtual void handleMessage(std::shared_ptr<Messaging::Message> message) = 0;
	};
}