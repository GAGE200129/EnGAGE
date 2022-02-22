#pragma once

#include <memory>
#include <cstdint>
#include <string>

#include "Game.hpp"
#include "Systems/MessageBus.hpp"
#include "Systems/InputSystem.hpp"

class GameEngine 
{
public:
	GameEngine(uint16_t width, uint16_t height, const std::string& title, std::shared_ptr<Game> game);
	~GameEngine();

private:
	void run();
private:
	std::shared_ptr<Game> mGame;
	Systems::MessageBus mMessageBus;
	Systems::InputSystem mInput;
};