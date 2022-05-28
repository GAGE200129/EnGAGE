#pragma once

#include "MessageTypes.hpp"
#include "MessageConstants.hpp"

namespace Core::Messenger
{
	
	void init();
	void shutdown();
	void flushQueued();
	void recieveMessage(Message* pMessage);
	void recieveMessage(MessageType type, void* data = nullptr);
	void queueMessage(Message* pMessage);
	void queueMessage(MessageType type, void* data = nullptr);
	bool processLuaMessage(lua_State* L);
	Message* queryMessage();
	unsigned int getMessageCount();
	Message* getMessages();

	template<MessageType Type, typename T>
	const T* messageCast(const Message* message)
	{
		if (message->type == Type)
		{
			return (const T*)message->message;
		}
		return nullptr;
	}
}