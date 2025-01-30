#include "Scene.h"

#include "Shader.h"

namespace scene {
	Scene::Scene() {
		comp::ModelLoader ml;
		auto car = ml.Load(".\\assets\\porsche_911_930_turbo_1975", "scene.gltf");
		car.SetPosition(glm::vec3(0, 2, 0));
		models.push_back(std::move(car));
		models.push_back(ml.Floor());
		models.push_back(ml.Wall(comp::ModelLoader::FRONT));
		models.push_back(ml.Wall(comp::ModelLoader::BACK));
		models.push_back(ml.Wall(comp::ModelLoader::LEFT));
		models.push_back(ml.Wall(comp::ModelLoader::RIGHT));
		models.push_back(comp::createModel());
	}

	void Scene::Render(render::Shader& shader) const {
		for (auto& model : models) {
			model.Render(shader);
		}
	}
}
