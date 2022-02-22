#pragma once

#include <cstdint>
#include <queue>
#include <memory>

#include "Messages/Message.hpp"

namespace Systems
{
	class MessageBus
	{
	public:
		std::shared_ptr<Message> queryMessage();
		void receiveMessage(const std::shared_ptr<Message>& message);

		bool empty() const;
	private:
		static constexpr uint8_t MAX_MESSAGES = 16;

		std::queue<std::shared_ptr<Message>> mMessages;

	};
}