#include "Engine.h"
#include <iostream>

Engine::Engine(Game& game) {
	std::cerr << "Engine is created for the game\n";
}

void Engine::Load() {
}

void Engine::Start() {
}

void Engine::Shutdown() {
}

Engine::~Engine() {
	std::cerr << "Engine is destroyed\n";
}
