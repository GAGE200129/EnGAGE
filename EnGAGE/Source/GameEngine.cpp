#include "GameEngine.hpp"

#include <iostream>

#include "FrameWork/Window.hpp"
#include "FrameWork/Input.hpp"

#include "Systems/Messages/KeyMessage.hpp"

using namespace FrameworkComponent;

GameEngine::GameEngine(uint16_t width, uint16_t height, const std::string& title, std::shared_ptr<Game> game) :
	mGame(game),
	mMessageBus(),
	mInput(&mMessageBus)
{
	using namespace std::placeholders;
	Window::init(width, height, title);
	Input::init(Window::getRawWindow());

	Input::setKeyfn(std::bind(&Systems::InputSystem::keyCallback, &mInput, _1, _2, _3, _4));
	Input::setButtonfn(std::bind(&Systems::InputSystem::buttonCallback, &mInput, _1, _2, _3));
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

			if (message->getType() == Systems::MessageType::KEY_RELEASED) {
				std::cout << "Pressed : " << std::static_pointer_cast<Systems::KeyPressed>(message)->getKeyCode() << "\n";
			}		
		}

		Window::swapBuffers();
		Window::pollEvents();
	}
}
