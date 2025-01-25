#pragma once
#include "types.h"

#include <vector>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include "assimp/types.h"

namespace render {
	class Shader;
	class Material;
}

namespace comp {

	struct NodeData {
		index_t i;
		glm::mat4 tx;
		std::vector<index_t> meshes;
		std::vector<index_t> children;
	};

	struct MeshData {
		index_t materialIndex;
		index_t baseVertex;
		index_t baseIndex;
		indexCount_t noOfIndices;
	};

	struct ModelData {
		std::vector<NodeData> nodes;
		std::vector<MeshData> meshes;

		std::vector<render::Material> materials;

		// complete data
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<index_t> indices;
	};

	constexpr unsigned int B_INDEX = 0;
	constexpr unsigned int B_POSITION = 1;
	constexpr unsigned int B_NORMAL = 2;
	constexpr unsigned int B_TEXCOORDS = 3;
	constexpr unsigned int B_B1 = 4;
	constexpr unsigned int B_B2 = 5;
	constexpr unsigned int NO_OF_BUFFERS = 6;

	class Model {
	private:
		float scale;
		glm::mat4 rotation;
		glm::mat4 position;
		glm::mat4 matrixCached;
		ModelData md;
		bool needsCleanUp = false;

		unsigned int VAO = 0;
		unsigned int vbos[NO_OF_BUFFERS] = { 0 };

		void calculateMatrix();

		void renderNode(render::Shader& shader, const glm::mat4& parentTx, const index_t n) const;

		void initGLComps() noexcept ;
		void cleanUp() noexcept;
	public:
		Model(ModelData&);

		Model();
		~Model();
		// do not support copy constructor, will need to create VAOs and VBOs
		// if needed create the Model using ModelData
		Model(const Model& o) = delete;
		// do not support copy assignment, see copy ctor
		Model& operator=(const Model& o) = delete;
		// move ctor
		Model(Model&& o) noexcept;
		// move assignment
		Model& operator=(Model&& o) noexcept;

		void SetScale(const float);
		void SetPosition(const glm::vec3& position);
		void Rotate(glm::vec3 axis, float angleInRadians);

		void Render(render::Shader& shader) const;
	};

	class ModelLoader {
	public:
		Model Floor();
		Model Load(std::string directory, std::string filepath);
	};

	Model createModel();
}
