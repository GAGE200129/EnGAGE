#pragma once

#include "Messages/Message.hpp"
#include "ECS/Coordinator.hpp"


namespace Systems
{
	class MessageBus
	{
	public:
		MessageBus(ECS::Coordinator& coordinator) : mCoordinator(coordinator) {}
		std::shared_ptr<Messaging::Message> queryMessage();
		void receiveMessage(const std::shared_ptr<Messaging::Message>& message);

		bool empty() const;

		inline const ECS::Coordinator& getCoordinator() const { return mCoordinator; }
	private:
		ECS::Coordinator& mCoordinator;
		std::queue<std::shared_ptr<Messaging::Message>> mMessages;
	};
}