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
	void recieveMessage(MessageType type, void* data = nullptr);
	void queueMessage(Message* pMessage);
	Request request(RequestType type, unsigned int size = 0, void* data = nullptr);
	Message* queryMessage();
	unsigned int getMessageCount();
	Message* getMessages();

}