#pragma once
#include "Game.h"

#include <memory>

#include "Window.h"
#include "GameLoop.h"
#include "entities.h"
#include "KeyMap.h"
#include "Shader.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "UnsafeAlloc.h"


typedef std::unique_ptr<render::Window> WindowPtr;
typedef std::unique_ptr<scene::Scene> ScenePtr;
typedef std::unique_ptr<comp::Model> ModelPtr;

namespace engine {

	struct ViewPort {
		int x, y;
		int width, height;
	};

	class Engine {
	public:
		UnsafeAlloc UnsafeAlloc_;
	private:
		static bool init;
		Game& game;
		WindowPtr window;
		GameLoop loop;
		entities::System entitySystem;
		engine::KeyMap keyMap;

		render::Shader lightSourceShader;
		render::Shader boldShader;
		render::Shader restShader;

		ModelPtr spawner;
		ScenePtr scene;

		SideViewCamera topView;
		SideViewCamera leftView;
		SideViewCamera frontView;
		Camera flyCamera;
		render::Light light;

		void render(ViewPort, CameraMatrixProvider&, render::Shader& , render::Shader&);
	public:
		Engine(Game& game);

		KeyMap& GetKeyMap();
		render::Window& Window() const;
		Camera& FlyCamera();
		entities::System& EntitySystem();
		const comp::Model& GetSpawner() const;
		render::Light& Light();

		void Load();
		void Start();
		void Update();
		void Render(double);

		void Mouse_callback(double xpos, double ypos);
		void Scroll_callback(double xoffset, double yoffset);

		~Engine();
	};
}

bool initGlfw();
