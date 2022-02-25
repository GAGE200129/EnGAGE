#pragma once

#include "pch.hpp"

#include "Message.hpp"
#include "MessageMacro.hpp"

namespace Messaging 
{
	class ConsoleInfo : public Message
	{
	public:
		ConsoleInfo(const std::string& message) : mMessage(message) {}
		EN_MESSAGE_TYPE(MessageType::CONSOLE_INFO)
		EN_MESSAGE_CATEGORY(MessageCategory::CONSOLE)

		inline const std::string& getMessage() const { return mMessage; }
	private:
		std::string mMessage;
	};

	class ConsoleWarn : public Message
	{
	public:
		ConsoleWarn(const std::string& message) : mMessage(message) {}
		EN_MESSAGE_TYPE(MessageType::CONSOLE_WARN)
		EN_MESSAGE_CATEGORY(MessageCategory::CONSOLE)

		inline const std::string& getMessage() const { return mMessage; }
	private:
		std::string mMessage;
	};

	class ConsoleError : public Message
	{
	public:
		ConsoleError(const std::string& message) : mMessage(message) {}
		EN_MESSAGE_TYPE(MessageType::CONSOLE_ERROR)
		EN_MESSAGE_CATEGORY(MessageCategory::CONSOLE)

		inline const std::string& getMessage() const { return mMessage; }
	private:
		std::string mMessage;
	};
}