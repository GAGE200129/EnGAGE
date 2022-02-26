#pragma once

#include "ECS/Coordinator.hpp"

class Game {
public:
	virtual ~Game() = default;
	virtual void init() = 0;

	inline void setCoordinator(ECS::Coordinator* coordinator) { mCoordinator = coordinator; }
protected:
	ECS::Coordinator* mCoordinator = nullptr;
};