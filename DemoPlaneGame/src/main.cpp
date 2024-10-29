/**
*/
#include "log.h"

#include "Game.h"
#include "Engine.h"

int main(int argc, char** argv) {
	LOG(INFO) << "This is the game\n";
	{
		Game game;
		{
			Engine engine(game);
		}
	}
	return 0;
}
