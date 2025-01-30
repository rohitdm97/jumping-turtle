#pragma once

#include <iostream>

#include "Model.h"

namespace entities {
	class System;
}

namespace engine {
	class Engine;
	class Camera;
	namespace camera {
		enum Movement;
	}
}

namespace render {
	class Window;
	class Light;
}

namespace game {
	class Action {
	public:
		virtual void Execute() = 0;
		virtual std::ostream& operator<<(std::ostream& os) {
			os << "Action{" << typeid(*this).name() << "}";
			return os;
		}
	};
	namespace actions {
		class ExitGame : public Action {
		public:
			render::Window& window;
			ExitGame(render::Window& w);
			void Execute();
		};
		class CameraMove : public Action {
		public:
			engine::camera::Movement direction;
			engine::Camera& camera;
			CameraMove(engine::camera::Movement, engine::Camera&);
			void Execute();
		};
		class CameraCaptureStats : public Action {
		public:
			engine::Camera& camera;
			CameraCaptureStats(engine::Camera& c);
			void Execute();
		};
		class SpawnParticle : public Action {
		public:
			engine::Camera& camera;
			entities::System& entities;
			const comp::Model& spawner;
			SpawnParticle(
				engine::Camera& c,
				entities::System& e,
				const comp::Model& s
			);
			void Execute();
		};
		class ChangeLightType : public Action {
		public:
			render::Light& light;
			ChangeLightType(render::Light& l);
			void Execute();
		};
	}
}
