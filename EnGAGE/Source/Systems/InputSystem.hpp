#pragma once

#include "SystemBase.hpp"

struct GLFWwindow;
namespace Systems
{
	class MessageBus;
	class InputSystem : public SystemBase
	{
	public:
		InputSystem(MessageBus* messageBus);

		void keyCallback(int key, int scancode, int action, int mods);
		void buttonCallback(int button, int action, int mods);

		virtual void handleMessage() override {};
	private:
		MessageBus* mMessageBus;
		
	};
}