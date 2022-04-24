#pragma once

namespace Core::Messaging
{
	static constexpr unsigned int BUFFER_SIZE = 64;
	static constexpr unsigned int MAX_MESSAGE_COUNT = 1024;


	enum class MessageType : unsigned int
	{
		KEY_PRESSED,
		KEY_RELEASED,
		BUTTON_PRESSED,
		BUTTON_RELEASED,
		CURSOR_MOVED,
		TOGGLE_CURSOR,
		WINDOW_RESIZED,
		COUNT
	};

	enum class RequestType : unsigned int
	{
		CURSOR_POS,
		CURSOR_DELTA,
		CURSOR_LOCKED
	};

	struct Message
	{
		MessageType type;
		char message[BUFFER_SIZE];
	};

	struct Request
	{
		char data[BUFFER_SIZE];
	};

	void init();
	void shutdown();
	void flushQueued();
	void recieveMessage(Message* pMessage);
	void queueMessage(Message* pMessage);
	Request request(RequestType type);
	Message* queryMessage();
	unsigned int getMessageCount();
	Message* getMessages();
	const char* getMessageName(MessageType type);
}