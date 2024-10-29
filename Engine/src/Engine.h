#pragma once
#include "Game.h"

class Engine {
public:
	Engine(Game& game);
	void Load();
	void Start();
	void Shutdown();
	~Engine();
};
