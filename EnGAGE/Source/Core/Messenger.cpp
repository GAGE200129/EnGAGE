#include "pch.hpp"
#include "Messenger.hpp"

#include "Input.hpp"
#include "Script.hpp"
#include "Physics.hpp"


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

	void queueMessage(Message* pMessage)
	{
		EN_ASSERT(pMessage != nullptr, "pMessage is null");
		EN_ASSERT(gQueuedMessageCount <= MAX_MESSAGE_COUNT, "Message overflow");
		Message* result = &gQueuedMessages[gQueuedMessageCount++];
		memcpy(result, pMessage, sizeof(Message));
	}

	Request request(RequestType type)
	{
		Request result;
		result.type = type;

		Input::onRequest(&result);
		Script::onRequest(&result);
		Physics::onRequest(&result);
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
