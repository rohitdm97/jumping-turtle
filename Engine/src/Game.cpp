#include "Game.h"
#include "log.h"

Game::Game() {
	LOG(TRACE) << "Game is created\n";
}

Game::~Game() {
	LOG(TRACE) << "Game is destroyed\n";
}
