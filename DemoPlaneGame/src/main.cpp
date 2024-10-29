#include <iostream>

#include "Game.h"
#include "Engine.h"

int main(int argc, char** argv) {
	std::cout << "This is the game" << std::endl;
	{
		Game game;
		{
			Engine engine(game);
		}
	}
	return 0;
}
