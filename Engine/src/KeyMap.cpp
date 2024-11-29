#include "KeyMap.h"

#define KEY_FIRST GLFW_KEY_SPACE
#define KEY_LAST GLFW_KEY_LAST

#include "Engine.h"

namespace engine {
	Action actions[] = {
#define X(a) a,
#include "actions.enums"
#undef X
	};

	KeyMap::KeyMap() {
		kToA = new Action[KEY_LAST - KEY_FIRST];
		aToK = new Key[ACTION_LAST];

		{
			SetIfNotPresent(GLFW_KEY_ESCAPE, EXIT);
			SetIfNotPresent(GLFW_KEY_W, MOVE_FORWARD);
			SetIfNotPresent(GLFW_KEY_A, MOVE_LEFT);
			SetIfNotPresent(GLFW_KEY_S, MOVE_BACKWARD);
			SetIfNotPresent(GLFW_KEY_D, MOVE_RIGHT);
			SetIfNotPresent(GLFW_KEY_SPACE, MOVE_UP);
			SetIfNotPresent(GLFW_KEY_LEFT_SHIFT, MOVE_DOWN);
		}
	}
	bool KeyMap::SetIfNotPresent(Key key, Action a) {
		if (kToA[key - KEY_FIRST] != 0 && aToK[a] != 0) {
			kToA[key - KEY_FIRST] = a;
			aToK[a] = key;
			return true;
		}
		return false;
	}

	void KeyMap::ProcessInput(Engine& engine, GLFWwindow* window) {
		for (unsigned int i = 0; i < ACTION_LAST; i++) {
			Key key = aToK[actions[i]];
			if (key != 0) {
				if (glfwGetKey(window, key) == GLFW_PRESS) {
					engine.ProcessInput(actions[i]);
				}
			}
		}
	}

	std::ostream& operator<<(std::ostream& os, Action a) {
		switch (a) {
#define X(a) case a: os << #a; return os;
#include "actions.enums"
#undef X
		default:
			throw new std::invalid_argument("Unknown enum Action " + std::to_string(a));
		}
	}

}
