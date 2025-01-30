#pragma once
#include "types.h"

#include <glm/glm.hpp>

#include "SparsedArray.h"

namespace entities {
	struct AIComponent {
		int dummy;
	};
	struct PhysicsComponent {
		glm::vec3 position;
		glm::vec3 velocity;
	};
	struct RenderComponent {
		int VAO;
	};
	class System {
	public:
		util::SparsedArray<AIComponent> ai;
		util::SparsedArray<PhysicsComponent> physics;
		util::SparsedArray<RenderComponent> renders;
	private:
		count_t nextID = 1;
	public:
		System();

		index_t CreateEntity();
	};

}
