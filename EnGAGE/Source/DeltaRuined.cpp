#include "pch.hpp"
#include "DeltaRuined.hpp"

#include "GameEngine.hpp"

void DeltaRuined::init()
{
	//mCoordinator->createEntity();
}

int main() 
{
	std::shared_ptr<DeltaRuined> pGame = std::make_shared<DeltaRuined>();
	GameEngine gameEngine(1600, 900, "Hello world", pGame);
}


