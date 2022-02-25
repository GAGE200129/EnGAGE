#pragma once

#include "Messages/Message.hpp"

namespace Systems
{
	class MessageBus
	{
	public:
		std::shared_ptr<Messaging::Message> queryMessage();
		void receiveMessage(const std::shared_ptr<Messaging::Message>& message);

		bool empty() const;
	private:
		static constexpr uint8_t MAX_MESSAGES = 16;

		std::queue<std::shared_ptr<Messaging::Message>> mMessages;

	};
}