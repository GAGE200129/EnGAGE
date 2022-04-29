#include "pch.hpp"
#include "Messenger.hpp"

#include "Input.hpp"
#include "Script.hpp"
#include "Physics.hpp"
#include "ECS.hpp"

namespace Core::Messenger
{
	static Message* gMessages;
	static Message* gQueuedMessages;
	static unsigned int gMessageCount;
	static unsigned int gQueuedMessageCount;

	void init()
	{
		gMessages = new Message[MAX_MESSAGE_COUNT];
		gQueuedMessages = new Message[MAX_MESSAGE_COUNT];
		gMessageCount = 0;
		gQueuedMessageCount = 0;
		memset(gMessages, 0, sizeof(Message) * MAX_MESSAGE_COUNT);
		memset(gQueuedMessages, 0, sizeof(Message) * MAX_MESSAGE_COUNT);
	}

	void shutdown()
	{
		delete[] gMessages;
		delete[] gQueuedMessages;
	}

	void flushQueued()
	{
		memcpy(&gMessages[gMessageCount], gQueuedMessages, sizeof(Message) * gQueuedMessageCount);
		gMessageCount += gQueuedMessageCount;
		gQueuedMessageCount = 0;
	}

	void recieveMessage(Message* pMessage)
	{
		EN_ASSERT(pMessage != nullptr, "pMessage is null");
		EN_ASSERT(gMessageCount <= MAX_MESSAGE_COUNT, "Message overflow");
		Message* result = &gMessages[gMessageCount++];
		memcpy(result, pMessage, sizeof(Message));
	}

	void recieveMessage(MessageType type, void* data)
	{
		EN_ASSERT(data != nullptr, "Data is null");
		EN_ASSERT(gMessageCount <= MAX_MESSAGE_COUNT, "Message overflow");
		Message* result = &gMessages[gMessageCount++];

		Message message;
		message.type = type;
		if(data != nullptr)
			memcpy(message.message, data, getMessageData(type).size);
		memcpy(result, &message, sizeof(Message));
	}

	void queueMessage(Message* pMessage)
	{
		EN_ASSERT(pMessage != nullptr, "pMessage is null");
		EN_ASSERT(gQueuedMessageCount <= MAX_MESSAGE_COUNT, "Message overflow");
		Message* result = &gQueuedMessages[gQueuedMessageCount++];
		memcpy(result, pMessage, sizeof(Message));
	}

	Request request(RequestType type, unsigned int size, void* data)
	{
		Request result;
		result.type = type;
		memcpy(result.data, data, size);

		if (Input::onRequest(&result)) { return result;  }
		if (Script::onRequest(&result)) { return result; }
		if (Physics::onRequest(&result)) { return result; }
		if (ECS::onRequest(&result)) { return result; }

		return result;
	}

	Message* queryMessage()
	{
		EN_ASSERT(gMessageCount <= MAX_MESSAGE_COUNT, "Message overflow");
		if (gMessageCount == 0) return nullptr;
		Message* result = &gMessages[gMessageCount-1];
		gMessageCount--;
		return result;
	}

	unsigned int getMessageCount()
	{
		return gMessageCount;
	}

	Message* getMessages()
	{
		return gMessages;
	}

	

}
