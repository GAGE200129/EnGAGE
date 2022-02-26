#pragma once


#include "Game.hpp"
#include "Systems/MessageBus.hpp"
#include "Systems/InputSystem.hpp"

#include "ECS/Coordinator.hpp"

class GameEngine 
{
public:
	GameEngine(uint16_t width, uint16_t height, const std::string& title, std::shared_ptr<Game> game);
	~GameEngine();

private:
	void initECS();
	void run();
private:
	std::shared_ptr<Game> mGame;
	ECS::Coordinator mCoordinator;
	Systems::MessageBus mMessageBus;
	Systems::InputSystem mInput;
};