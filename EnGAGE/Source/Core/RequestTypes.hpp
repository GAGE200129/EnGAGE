#pragma once

#include "MessageConstants.hpp"

namespace Core
{
	enum class RequestType : unsigned int
	{
		CURSOR_POS,
		CURSOR_DELTA,
		CURSOR_LOCKED,

		NEW_SCRIPT,
		NEW_RIGID_BODY,
	};

	struct Request
	{
		RequestType type;
		unsigned char data[Messenger::BUFFER_SIZE];
	};

}