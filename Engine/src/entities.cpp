#include "entities.h"

namespace entities {
	System::System() {
	}

	index_t System::CreateEntity() {
		const index_t result = nextID;
		ai[result] = AIComponent{};
		physics[result] = PhysicsComponent{};
		renders[result] = RenderComponent{};
		nextID++;
		return result;
	}

}
