#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

namespace engine {

	class Engine;
	typedef int Key;

	enum Action {
#define X(a) a,
#include "actions.enums"
#undef X
		ACTION_LAST,
	};

	std::ostream& operator<<(std::ostream& os, Action a);

	Action actions[];

	class KeyMap {
	private:
		// keyToActionMapping
		Action* kToA;
		// actionToKeyMapping
		Key* aToK;
	public:
		KeyMap();
		bool SetIfNotPresent(Key key, Action a);
		void ProcessInput(Engine& engine, GLFWwindow* window);
	};
}
