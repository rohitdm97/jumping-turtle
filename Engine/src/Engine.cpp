#include "Engine.h"
#include "log.h"

Engine::Engine(Game& game) {
	LOG(TRACE) << "Engine is created for the game\n";
}

void Engine::Load() {
}

void Engine::Start() {
}

void Engine::Shutdown() {
}

Engine::~Engine() {
	LOG(TRACE) << "Engine is destroyed\n";
}
