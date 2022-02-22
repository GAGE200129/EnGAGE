#include "DeltaRuined.hpp"

#include <iostream>

#include "GameEngine.hpp"

void DeltaRuined::init()
{
	std::cout << "Running !\n";
}

int main() 
{
	std::shared_ptr<DeltaRuined> pGame = std::make_shared<DeltaRuined>();
	GameEngine gameEngine(1600, 900, "Hello world", pGame);
}


