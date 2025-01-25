#include "Scene.h"

#include "Shader.h"

namespace scene {
	Scene::Scene() {
		comp::ModelLoader ml;
		//models.push_back(ml.Load("..\\assets\\porsche_911_930_turbo_1975", "scene.gltf"));
		models.push_back(ml.Floor());
		models.push_back(comp::createModel());
	}

	void Scene::Render(render::Shader& shader) const {
		for (auto& model : models) {
			model.Render(shader);
		}
	}
}
