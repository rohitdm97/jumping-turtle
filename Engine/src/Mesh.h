#pragma once

#include <vector>
#include <string>

#include "glm/glm.hpp"

#include <Texture.h>

namespace render {
	class Shader;
}

namespace comp {
	constexpr unsigned int INVALID_MATERIAL_INDEX = -1L >> 1;

	constexpr unsigned int B_INDEX = 0;
	constexpr unsigned int B_POSITION = 1;
	constexpr unsigned int B_NORMAL = 2;
	constexpr unsigned int B_TEXCOORDS = 3;
	constexpr unsigned int B_B1 = 4;
	constexpr unsigned int B_B2 = 5;
	constexpr unsigned int NO_OF_BUFFERS = 6;

	struct MeshEntry {
		unsigned int numOfIndices;
		unsigned int baseVertex;
		unsigned int baseIndex;
		unsigned int materialIndex = INVALID_MATERIAL_INDEX;
	};

	typedef struct {
		std::vector<MeshEntry> meshes;
		std::vector<render::Texture> textures;

		std::vector<unsigned int> indices;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
	} MeshData;
	class Mesh {
	private:
		MeshData md;

		unsigned int VAO = 0;
		unsigned int vbos[NO_OF_BUFFERS] = { 0 };
	public:
		/** default no-args constructor
		*/
		Mesh();
		/** pseudo copy constructor used by loader
		*/
		Mesh(MeshData& md);
		~Mesh();
		// copy constructor
		Mesh(const Mesh& other) = delete;
		// copy assignment
		Mesh& operator=(const Mesh& other) = delete;
		// move constructor
		Mesh(Mesh&& o) noexcept;
		// move assignment
		Mesh& operator=(Mesh&& o) noexcept;

		void Attach(render::Shader& shader);
		void Render();

		friend class MeshLoader;
		friend Mesh createMesh();
	};
	Mesh createMesh();

	class MeshLoader {
	public:
		Mesh Load(std::string directory, std::string filepath);
	};
}
