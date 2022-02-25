#pragma once

#include "MessageType.hpp"
#include "MessageCategory.hpp"

namespace Messaging
{

	class Message 
	{
	public:
		virtual MessageType getType() const = 0;
		virtual MessageCategory getCategory() const = 0;

		inline bool isType(MessageType type) const { return getType() == type; }
		inline bool inCategory(MessageCategory category) const { return getCategory() == category; }
	};
}