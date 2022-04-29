#pragma once

#include "MessageConstants.hpp"

namespace Core
{
	enum class RequestType : unsigned int
	{
		CURSOR_LOCKED,

		NEW_SCRIPT,
		NEW_RIGID_BODY,
		ENTITY_COMPONENT
	};

	struct Request
	{
		RequestType type;
		unsigned char data[Messenger::BUFFER_SIZE];
	};

}