#pragma once

#include "ECS/System.hpp"

struct GLFWwindow;
namespace Systems
{
	class MessageBus;
	class InputSystem : public ECS::System
	{
	public:
		InputSystem(MessageBus& messageBus);

		void keyCallback(int key, int scancode, int action, int mods);
		void buttonCallback(int button, int action, int mods);
		void cursorPosCallback(double x, double y);

		virtual void handleMessage(std::shared_ptr<Messaging::Message> message) override;
	private:
		MessageBus& mMessageBus;
		
	};
}