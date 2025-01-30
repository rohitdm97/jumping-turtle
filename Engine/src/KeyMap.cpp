#include "KeyMap.h"

#define KEY_FIRST GLFW_KEY_SPACE
#define KEY_LAST GLFW_KEY_LAST

#include "Engine.h"

namespace engine {
//	Action actions[] = {
//#define X(a) a,
//#include "actions.enums"
//#undef X
//	};

	KeyMap::KeyMap() {
	}

	KeyMap::KeyMap(engine::Engine& engine) {
		keyToAction.resize(KEY_LAST - KEY_FIRST);
		engine.UnsafeAlloc_.ExitGame_ = new game::actions::ExitGame(engine.Window());
		engine.UnsafeAlloc_.CameraMoveForward_ = new game::actions::CameraMove(engine::camera::FORWARD, engine.FlyCamera());
		engine.UnsafeAlloc_.CameraMoveLeft_ = new game::actions::CameraMove(engine::camera::LEFT, engine.FlyCamera());
		engine.UnsafeAlloc_.CameraMoveBackward_ = new game::actions::CameraMove(engine::camera::BACKWARD, engine.FlyCamera());
		engine.UnsafeAlloc_.CameraMoveRight_ = new game::actions::CameraMove(engine::camera::RIGHT, engine.FlyCamera());
		engine.UnsafeAlloc_.CameraCaptureStats_ = new game::actions::CameraCaptureStats(engine.FlyCamera());
		engine.UnsafeAlloc_.SpawnParticle_ = new game::actions::SpawnParticle(
			engine.FlyCamera(),
			engine.EntitySystem(),
			engine.GetSpawner()
		);
		engine.UnsafeAlloc_.ChangeLightType_ = new game::actions::ChangeLightType(engine.Light());

		{
			SetIfNotPresent(GLFW_KEY_ESCAPE, engine.UnsafeAlloc_.ExitGame_);
			SetIfNotPresent(GLFW_KEY_W, engine.UnsafeAlloc_.CameraMoveForward_);
			SetIfNotPresent(GLFW_KEY_A, engine.UnsafeAlloc_.CameraMoveLeft_);
			SetIfNotPresent(GLFW_KEY_S, engine.UnsafeAlloc_.CameraMoveBackward_);
			SetIfNotPresent(GLFW_KEY_D, engine.UnsafeAlloc_.CameraMoveRight_);
			SetIfNotPresent(GLFW_KEY_SPACE, engine.UnsafeAlloc_.SpawnParticle_);
			//SetIfNotPresent(GLFW_KEY_LEFT_SHIFT, MOVE_DOWN);
			SetIfNotPresent(GLFW_KEY_C, engine.UnsafeAlloc_.CameraCaptureStats_);
			// todo change this to callback instead active polling
			SetIfNotPresent(GLFW_KEY_L, engine.UnsafeAlloc_.ChangeLightType_);
		}
	}

	bool KeyMap::SetIfNotPresent(Key key, void* a) {
		assert(key < KEY_LAST && "invalid key must be less than KEY_LAST");
		assert(key >= KEY_FIRST && "invalid key must be geq than KEY_FIRST");
		const void* oldAction = keyToAction[key - KEY_FIRST];
		if (oldAction == nullptr && actionToKey.find(a) == actionToKey.end()) {
			keyToAction[key - KEY_FIRST] = a;
			actionToKey[a] = key;
			return true;
		}
		return false;
	}

	void KeyMap::ProcessInput(Engine& engine, GLFWwindow* window) {
		for (auto const& [action, key] : actionToKey) {
			if (glfwGetKey(window, key) == GLFW_PRESS) {
				static_cast<game::Action*>(action)->Execute();
			}
		}
	}

//	std::ostream& operator<<(std::ostream& os, Action a) {
//		switch (a) {
//#define X(a) case a: os << #a; return os;
//#include "actions.enums"
//#undef X
//		default:
//			throw new std::invalid_argument("Unknown enum Action " + std::to_string(a));
//		}
//	}

}
