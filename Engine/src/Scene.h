#pragma once

#include <vector>

#include <Model.h>

namespace render {
	class Shader;
}

namespace scene {
	class Scene;

	class SceneLoader {
	public:
		SceneLoader();

		Scene Load(const std::string filename);
	};

	class Scene {
	private:
		std::vector<comp::Model> models;
	public:
		Scene();
		~Scene() = default;
		Scene(const Scene& s) = default;
		Scene& operator=(const Scene& s) = default;
		Scene(Scene&& s) = default;
		Scene& operator=(Scene&& s) = default;

		void Render(render::Shader& shader) const;
	};
}
