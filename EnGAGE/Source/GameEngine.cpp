#include "pch.hpp"
#include "GameEngine.hpp"


#include "Framework/Window.hpp"
#include "Framework/Input.hpp"
#include "Framework/Log.hpp"

#include "Systems/Messages/KeyMessage.hpp"
#include "Systems/Messages/MouseMessage.hpp"

using namespace FrameworkComponent;

GameEngine::GameEngine(uint16_t width, uint16_t height, const std::string& title, std::shared_ptr<Game> game) :
	mGame(game),
	mMessageBus(),
	mInput(mMessageBus)
{
	using namespace std::placeholders;
	Window::init(width, height, title);
	Log::init();
	Input::init(Window::getRawWindow());


	Input::setKeyfn(std::bind(&Systems::InputSystem::keyCallback, &mInput, _1, _2, _3, _4));
	Input::setButtonfn(std::bind(&Systems::InputSystem::buttonCallback, &mInput, _1, _2, _3));
	Input::setCursorPosFn(std::bind(&Systems::InputSystem::cursorPosCallback, &mInput, _1, _2));
	game->init();
	this->run();
}

GameEngine::~GameEngine()
{
	Window::destroy();
}

void GameEngine::run()
{
	while (!Window::closeRequested()) {

		while (!mMessageBus.empty()) {
			auto message = mMessageBus.queryMessage();


			if (message->isType(Messaging::MessageType::KEY_PRESSED)) {
				Messaging::KeyPressed& keyPressed = (Messaging::KeyPressed&)*message;

				if (keyPressed.getKeyCode() == FrameworkComponent::InputCodes::KEY_ESCAPE) {
					mMessageBus.receiveMessage(std::make_shared<Messaging::ToggleMouse>());
				}
			}		
			if (message->isType(Messaging::MessageType::MOUSE_MOVED)) {
				Messaging::MouseMoved& mouseMoved = (Messaging::MouseMoved&)*message;
				std::cout << "X: " << std::fixed << mouseMoved.getX() << " | Y: " << mouseMoved.getY() << "\n";
			}

			mInput.handleMessage(message);
		}

		Window::swapBuffers();
		Window::pollEvents();
	}
}
