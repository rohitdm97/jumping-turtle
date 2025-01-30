#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <Action.h>

namespace engine {

	class Engine;
	typedef int Key;

//	enum Action {
//#define X(a) a,
//#include "actions.enums"
//#undef X
//		ACTION_LAST,
//	};
//
//	std::ostream& operator<<(std::ostream& os, Action a);

	class KeyMap {
	private:
		// keyToActionMapping
		std::vector<void*> keyToAction;
		// actionToKeyMapping
		std::map<void*, Key> actionToKey;
	public:
		KeyMap();
		KeyMap(Engine&);
		bool SetIfNotPresent(Key key, void*);
		void ProcessInput(Engine& engine, GLFWwindow* window);
	};
}
