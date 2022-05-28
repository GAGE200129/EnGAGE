#include "pch.hpp"
#include "Messenger.hpp"


namespace Core::Messenger
{
	static Scope<Message[]> gMessages;
	static Scope<Message[]> gQueuedMessages;
	static unsigned int gMessageCount;
	static unsigned int gQueuedMessageCount;

	void init()
	{
		gMessages = createScope<Message[]>(MAX_MESSAGE_COUNT);
		gQueuedMessages = createScope<Message[]>(MAX_MESSAGE_COUNT);
		gMessageCount = 0;
		gQueuedMessageCount = 0;
		memset(gMessages.get(), 0, sizeof(Message) * MAX_MESSAGE_COUNT);
		memset(gQueuedMessages.get(), 0, sizeof(Message) * MAX_MESSAGE_COUNT);
	}

	void shutdown()
	{
	}

	void flushQueued()
	{
		memcpy(&gMessages[gMessageCount], gQueuedMessages.get(), sizeof(Message) * gQueuedMessageCount);
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

	void queueMessage(MessageType type, void* data)
	{
		EN_ASSERT(gQueuedMessageCount <= MAX_MESSAGE_COUNT, "Message overflow");
		Message* result = &gQueuedMessages[gQueuedMessageCount++];

		Message message;
		message.type = type;
		if (data != nullptr)
			memcpy(message.message, data, getMessageData(type).size);
		memcpy(result, &message, sizeof(Message));
	}

	bool processLuaMessage(lua_State* L)
	{
		bool status = false;
		Message message;
		message.type = (MessageType)lua_tointeger(L, 1);

		//switch (message.type)
		//{
		//case MessageType::TOGGLE_CURSOR:
		//{
		//
		//	status = true;
		//	break;
		//}
		//}

		Messenger::queueMessage(&message);

		return status;
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
		return gMessages.get();
	}

	

}
