#pragma once
#include "Game.h"

#include <memory>

#include "Window.h"
#include "GameLoop.h"
#include "KeyMap.h"
#include "Shader.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"


typedef std::unique_ptr<render::Window> WindowPtr;
typedef std::unique_ptr<scene::Scene> ScenePtr;

namespace engine {

	struct ViewPort {
		int x, y;
		int width, height;
	};

	class Engine {
	private:
		static bool init;
		Game& game;
		WindowPtr window;
		GameLoop loop;
		KeyMap keyMap;

		render::Shader lightSourceShader;
		render::Shader boldShader;
		render::Shader restShader;

		ScenePtr scene;

		SideViewCamera topView;
		SideViewCamera leftView;
		SideViewCamera frontView;
		Camera flyCamera;
		render::Light light;

		void render(ViewPort, CameraMatrixProvider&);
	public:
		Engine(Game& game);

		KeyMap& KeyMap();
		render::Window& Window() const;

		bool LoadGLAD();
		void Load();
		void Start();
		void ProcessInput(Action a);
		void Update();
		void Render();

		void Mouse_callback(double xpos, double ypos);
		void Scroll_callback(double xoffset, double yoffset);

		~Engine();
	};
}

bool initGlfw();
