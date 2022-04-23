#include "pch.hpp"
#include "Messaging.hpp"

#include "Input.hpp"

#define TO_STRING(x) case MessageType::x: return #x

namespace Core::Messaging
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
		Input::request(type, &result);
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

	const char* getMessageName(MessageType type)
	{
		switch (type)
		{
			TO_STRING(KEY_PRESSED);
			TO_STRING(KEY_RELEASED);
			TO_STRING(BUTTON_PRESSED);
			TO_STRING(BUTTON_RELEASED);
			TO_STRING(CURSOR_MOVED);
			TO_STRING(TOGGLE_CURSOR);
		}
		EN_ASSERT(false, "Unknown message type: {}", (unsigned int)type);
		return "";
	}

}
