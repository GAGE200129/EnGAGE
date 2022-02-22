#include "InputSystem.hpp"

#include "MessageBus.hpp"

#include "Messages/KeyMessage.hpp"
#include "Messages/MouseMessage.hpp"

#include "../FrameWork/Input.hpp"

Systems::InputSystem::InputSystem(MessageBus* messageBus) :
	mMessageBus(messageBus)
{

}

void Systems::InputSystem::keyCallback(int key, int scancode, int action, int mods)
{
	if (action == FrameworkComponent::Input::PRESS) {
		mMessageBus->receiveMessage(std::make_shared<KeyPressed>(key));
	}
	else if (action == FrameworkComponent::Input::RELEASE) {
		mMessageBus->receiveMessage(std::make_shared<KeyReleased>(key));
	}
}

void Systems::InputSystem::buttonCallback(int button, int action, int mods)
{
	if (action == FrameworkComponent::Input::PRESS) {
		mMessageBus->receiveMessage(std::make_shared<MouseButtonPressed>(button));
	}
	else if (action == FrameworkComponent::Input::RELEASE) {
		mMessageBus->receiveMessage(std::make_shared<MouseButtonReleased>(button));
	}
}

