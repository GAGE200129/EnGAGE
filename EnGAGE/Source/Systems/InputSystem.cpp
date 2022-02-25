#include "pch.hpp"
#include "InputSystem.hpp"

#include "MessageBus.hpp"

#include "Messages/KeyMessage.hpp"
#include "Messages/MouseMessage.hpp"

#include "../FrameWork/Input.hpp"

using namespace FrameworkComponent;
using namespace Messaging;
using namespace Systems;

Systems::InputSystem::InputSystem(MessageBus& messageBus) :
	mMessageBus(messageBus)
{

}

void Systems::InputSystem::keyCallback(int key, int scancode, int action, int mods)
{
	if (action == Input::PRESS) {
		mMessageBus.receiveMessage(std::make_shared<KeyPressed>(key));
	}
	else if (action == Input::RELEASE) {
		mMessageBus.receiveMessage(std::make_shared<KeyReleased>(key));
	}
}

void Systems::InputSystem::buttonCallback(int button, int action, int mods)
{
	if (action == Input::PRESS) {
		mMessageBus.receiveMessage(std::make_shared<MouseButtonPressed>(button));
	}
	else if (action == Input::RELEASE) {
		mMessageBus.receiveMessage(std::make_shared<MouseButtonReleased>(button));
	}
}

void Systems::InputSystem::cursorPosCallback(double x, double y)
{
	mMessageBus.receiveMessage(std::make_shared<MouseMoved>(x, y));
}

void Systems::InputSystem::handleMessage(std::shared_ptr<Message> message)
{
	if (message->inCategory(MessageCategory::INPUT)) {
		if (message->isType(MessageType::MOUSE_DISABLE)) {
			Input::disableCursor();
		}
		else if (message->isType(MessageType::MOUSE_ENABLE)) {
			Input::enableCursor();
		}
		else if (message->isType(MessageType::MOUSE_TOGGLE)) {
			Input::toggleCursor();
		}
	}
}

