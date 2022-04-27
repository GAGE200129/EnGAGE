#pragma once

#include "MessageTypes.hpp"
#include "RequestTypes.hpp"
#include "MessageConstants.hpp"

namespace Core::Messenger
{
	
	void init();
	void shutdown();
	void flushQueued();
	void recieveMessage(Message* pMessage);
	void queueMessage(Message* pMessage);
	Request request(RequestType type);
	Message* queryMessage();
	unsigned int getMessageCount();
	Message* getMessages();

}