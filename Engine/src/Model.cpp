#include "Model.h"
#include "types.h"
#include "log.h"

#include <glad/glad.h>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/types.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Material.h"

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from) {
	glm::mat4 to{};

	to[0][0] = from->a1; to[0][1] = from->b1;  to[0][2] = from->c1; to[0][3] = from->d1;
	to[1][0] = from->a2; to[1][1] = from->b2;  to[1][2] = from->c2; to[1][3] = from->d2;
	to[2][0] = from->a3; to[2][1] = from->b3;  to[2][2] = from->c3; to[2][3] = from->d3;
	to[3][0] = from->a4; to[3][1] = from->b4;  to[3][2] = from->c4; to[3][3] = from->d4;

	return to;
}

namespace static_data {
	namespace model {
		glm::vec3 g_positions[] = {
			glm::vec3(0.0f, 2, 0) + glm::vec3(0.5f, -0.5f, 0.0f),  // bottom right
			glm::vec3(0.0f, 2, 0) + glm::vec3(-0.5f, -0.5f, 0.0f),  // bottom left
			glm::vec3(0.0f, 2, 0) + glm::vec3(0.0f,  0.5f, 0.0f),  // top 
		};

		glm::vec3 g_normals[] = {
			glm::vec3(0.0f,  0.0f, 1.0f),  // bottom right
			glm::vec3(0.0f,  0.0f, 1.0f),  // bottom left
			glm::vec3(0.0f,  0.0f, 1.0f),  // top 
		};

		glm::vec2 g_texCoords[] = {
			glm::vec2(0.0f,  0.0f),  // bottom right
			glm::vec2(1.0f,  0.0f),  // bottom left
			glm::vec2(0.5f,  1.0f),  // top 
		};

		unsigned int g_indices[] = {
			0, 2, 1,
		};
	}
}

namespace comp {
	Model createModel() {
		ModelData md;
		md.nodes.reserve(1);
		md.meshes.reserve(1);

		const auto vCount = 3;
		const auto iCount = 3;

		MeshData mesh0{};
		mesh0.materialIndex = 0;
		mesh0.noOfIndices = iCount;
		mesh0.baseVertex = 0;
		mesh0.baseIndex = 0;
		md.meshes.push_back(mesh0);

		md.positions.reserve(vCount);
		md.normals.reserve(vCount);
		md.texCoords.reserve(vCount);
		md.indices.reserve(iCount);

		const aiVector3D Zero(0.0f, 0.0f, 0.0f);
		for (unsigned int j = 0; j < vCount; j++) {
			md.positions.push_back(static_data::model::g_positions[j]);
			md.normals.push_back(static_data::model::g_normals[j]);
			md.texCoords.push_back(static_data::model::g_texCoords[j]);
		}
		for (unsigned int j = 0; j < iCount; j++) {
			md.indices.push_back(static_data::model::g_indices[j]);
		}

		auto whileTile = render::material::whiteTile();
		md.materials.push_back(whileTile);

		NodeData node0;
		node0.meshes.push_back(0);
		node0.tx = glm::mat4(1.0);
		md.nodes.push_back(node0);

		return Model(md);
	}
	Model* CreateParticleSpawner() {
		glm::vec3 l_positions[] = {
			glm::vec3(0.5f, -0.5f, 0.0f),  // bottom right
			glm::vec3(-0.5f, -0.5f, 0.0f),  // bottom left
			glm::vec3(0.0f,  0.5f, 0.0f),  // top 
		};

		glm::vec3 l_normals[] = {
			glm::vec3(0.0f,  0.0f, 1.0f),  // bottom right
			glm::vec3(0.0f,  0.0f, 1.0f),  // bottom left
			glm::vec3(0.0f,  0.0f, 1.0f),  // top 
		};

		glm::vec2 l_texCoords[] = {
			glm::vec2(0.0f,  0.0f),  // bottom right
			glm::vec2(1.0f,  0.0f),  // bottom left
			glm::vec2(0.5f,  1.0f),  // top 
		};

		unsigned int l_indices[] = {
			0, 2, 1,
		};

		ModelData md;
		md.nodes.reserve(1);
		md.meshes.reserve(1);

		const auto vCount = 3;
		const auto iCount = 3;

		MeshData mesh0{};
		mesh0.materialIndex = 0;
		mesh0.noOfIndices = iCount;
		mesh0.baseVertex = 0;
		mesh0.baseIndex = 0;
		md.meshes.push_back(mesh0);

		md.positions.reserve(vCount);
		md.normals.reserve(vCount);
		md.texCoords.reserve(vCount);
		md.indices.reserve(iCount);

		const aiVector3D Zero(0.0f, 0.0f, 0.0f);
		for (unsigned int j = 0; j < vCount; j++) {
			md.positions.push_back(l_positions[j]);
			md.normals.push_back(l_normals[j]);
			md.texCoords.push_back(l_texCoords[j]);
		}
		for (unsigned int j = 0; j < iCount; j++) {
			md.indices.push_back(l_indices[j]);
		}

		auto whileTile = render::material::whiteTile();
		md.materials.push_back(whileTile);

		NodeData node0;
		node0.meshes.push_back(0);
		node0.tx = glm::mat4(1.0);
		md.nodes.push_back(node0);

		auto result = new Model(md);
		result->SetScale(0.1f);
		return result;
	}
}

namespace comp {
}

void printNode(std::string prefix, aiNode* node) {
	if (node == nullptr) {
		return;
	}
	std::cout << prefix << "name: " << std::string(node->mName.C_Str()) << "\n";
	std::cout << prefix << "mMeshes: " << ((unsigned int)node->mNumMeshes) << "\n";
	if (node->mMeshes != nullptr) {
		std::cout << prefix << "meshe indices: \n";
		std::cout << prefix << "[";
		for (index_t i = 0; i < node->mNumMeshes; i++) {
			std::cout << node->mMeshes[i] << ", ";
		}
		std::cout << "]\n";
	}
	std::cout << prefix << "children count: " << node->mNumChildren << "\n";
	for (index_t i = 0; i < node->mNumChildren; i++) {
		printNode(prefix + "--", node->mChildren[i]);
	}
}


namespace comp {
	void Model::calculateMatrix() {
		this->matrixCached = position * rotation * glm::scale(glm::mat4(1.0f), glm::vec3(scale));
	}

	Model::Model(ModelData& md) {
		this->md = std::move(md);
		this->position = glm::mat4(1.0f);
		this->rotation = glm::mat4(1.0f);
		this->scale = 1.0f;
		this->calculateMatrix();
		initGLComps();
	}

	Model::Model() {
		this->position = glm::mat4(1.0f);
		this->rotation = glm::mat4(1.0f);
		this->scale = 1.0f;
		this->calculateMatrix();
	}

	void Model::initGLComps() noexcept {
		this->needsCleanUp = true;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(comp::NO_OF_BUFFERS, vbos);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[B_POSITION]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(this->md.positions[0]) * this->md.positions.size(), &this->md.positions[0], GL_STATIC_DRAW);
		glVertexAttribPointer(render::attribs::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(render::attribs::POSITION);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[B_NORMAL]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(this->md.normals[0]) * this->md.normals.size(), &this->md.normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(render::attribs::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(render::attribs::NORMAL);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[B_TEXCOORDS]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(this->md.texCoords[0]) * this->md.texCoords.size(), &this->md.texCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(render::attribs::TEXUV, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(render::attribs::TEXUV);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[B_INDEX]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->md.indices[0]) * this->md.indices.size(), &this->md.indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Model::cleanUp() noexcept {
		// todo delete
		if (needsCleanUp) {
			glDeleteBuffers(NO_OF_BUFFERS, vbos);
			glDeleteVertexArrays(1, &VAO);
			needsCleanUp = false;
		}
	}

	Model::~Model() {
		cleanUp();
	}

	Model::Model(Model&& o) noexcept {
		this->md = std::move(o.md);

		this->position = std::move(o.position);
		this->rotation = std::move(o.rotation);
		this->scale = std::move(o.scale);
		this->matrixCached = std::move(o.matrixCached);

		this->needsCleanUp = true;
		o.needsCleanUp = false;

		this->VAO = o.VAO;
		o.VAO = 0;
		for (unsigned int i = 0; i < NO_OF_BUFFERS; i++) {
			this->vbos[i] = o.vbos[i];
			o.vbos[i] = 0;
		}
	}

	Model& Model::operator=(Model&& o) noexcept {
		if (this != &o) {
			this->md = std::move(o.md);

			this->position = std::move(o.position);
			this->rotation = std::move(o.rotation);
			this->scale = std::move(o.scale);
			this->matrixCached = std::move(o.matrixCached);

			this->needsCleanUp = true;
			o.needsCleanUp = false;

			this->VAO = o.VAO;
			o.VAO = 0;
			for (unsigned int i = 0; i < NO_OF_BUFFERS; i++) {
				this->vbos[i] = o.vbos[i];
				o.vbos[i] = 0;
			}
		}
		return *this;
	}

	const unsigned int& Model::GetVAO() const {
		return VAO;
	}

	void Model::SetScale(const float scale_) {
		this->scale = scale_;
		this->calculateMatrix();
	}

	void Model::SetPosition(const glm::vec3& position_) {
		this->position = glm::translate(glm::mat4(1.0f), position_);
		this->calculateMatrix();
	}
	void Model::Rotate(glm::vec3 axis, float angleInRadians) {
		this->rotation = glm::rotate(this->rotation, angleInRadians, axis);
		this->calculateMatrix();
	}

	void Model::renderNode(render::Shader& shader, const glm::mat4& parentTx, const index_t n) const {
		const glm::mat4 thisTx = parentTx * md.nodes[n].tx;
		shader.Uniforms().SetMat4("Mesh", thisTx);

		// draw call
		for (const index_t mIdx : md.nodes[n].meshes) {
			const auto tIdx = md.meshes[mIdx].materialIndex;
			if (tIdx < md.materials.size()) {
				assert(tIdx >= 0 && "materialIndex is negative");
				assert(tIdx < md.materials.size() && "invalid materialIndex");
				md.materials[tIdx].Activate("material", shader);
			}

			glDrawElementsBaseVertex(GL_TRIANGLES,
				md.meshes[mIdx].noOfIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * md.meshes[mIdx].baseIndex),
				md.meshes[mIdx].baseVertex
			);
		}

		for (const index_t cIdx : md.nodes[n].children) {
			renderNode(shader, thisTx, cIdx);
		}
	}

	void Model::Render(render::Shader& shader) const {
		glBindVertexArray(VAO);
		shader.Uniforms().SetMat4("Model", matrixCached);
		renderNode(shader, glm::mat4(1.0f), 0);
		glBindVertexArray(0);
	}

	void addNodeDataToList(std::vector<NodeData>& result, std::map<aiNode*, index_t>& nodesToIdxMapping, aiNode* node) {
		if (node == nullptr) {
			return;
		}
		const index_t newIndex = (index_t) result.size();
		NodeData nd;
		nd.tx = aiMatrix4x4ToGlm(&node->mTransformation);
		nd.meshes.reserve(node->mNumMeshes);
		for (index_t i = 0; i < node->mNumMeshes; i++) {
			nd.meshes.push_back(node->mMeshes[i]);
		}
		result.push_back(nd);
		nodesToIdxMapping[node] = newIndex;
		if (node->mChildren == nullptr) {
			return;
		}
		for (index_t i = 0; i < node->mNumChildren; i++) {
			addNodeDataToList(result, nodesToIdxMapping, node->mChildren[i]);
		}
	}

	void addChildrenToNodeData(std::vector<NodeData>& nodes, std::map<aiNode*, index_t>& nodesToIdxMapping, aiNode* node) {
		if (node == nullptr) {
			return;
		}
		if (node->mChildren == nullptr) {
			return;
		}
		assert(nodesToIdxMapping.find(node) != nodesToIdxMapping.end());
		const index_t idx = nodesToIdxMapping[node];
		NodeData& nd = nodes[idx];
		nd.children.reserve(node->mNumChildren);
		for (index_t i = 0; i < node->mNumChildren; i++) {
			assert(nodesToIdxMapping.find(node->mChildren[i]) != nodesToIdxMapping.end());
			const index_t cIdx = nodesToIdxMapping[node->mChildren[i]];
			nd.children.push_back(cIdx);
		}
		for (index_t i = 0; i < node->mNumChildren; i++) {
			addChildrenToNodeData(nodes, nodesToIdxMapping, node->mChildren[i]);
		}
	}

	Model ModelLoader::Floor() {
		const glm::vec3 l_positions[] = {
			glm::vec3(-10.0f, -0.001f, -10.0f),
			glm::vec3(-10.0f, -0.001f, +10.0f),
			glm::vec3(+10.0f, -0.001f, +10.0f),
			glm::vec3(+10.0f, -0.001f, -10.0f),
		};

		const glm::vec3 l_normals[] = {
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
		};

		const glm::vec2 l_texCoords[] = {
			glm::vec2(0.0f,  0.0f),
			glm::vec2(1.0f,  0.0f),
			glm::vec2(1.0f,  1.0f),
			glm::vec2(0.0f,  1.0f),
		};

		const unsigned int l_indices[] = {
			0, 1, 2,
			0, 2, 3,
		};

		ModelData md;
		md.nodes.reserve(1);
		md.meshes.reserve(1);

		const auto vCount = 4;
		const auto iCount = 6;

		MeshData mesh0{};
		mesh0.materialIndex = 0;
		mesh0.noOfIndices = iCount;
		mesh0.baseVertex = 0;
		mesh0.baseIndex = 0;
		md.meshes.push_back(mesh0);

		md.positions.reserve(vCount);
		md.normals.reserve(vCount);
		md.texCoords.reserve(vCount);
		md.indices.reserve(iCount);

		const aiVector3D Zero(0.0f, 0.0f, 0.0f);
		for (unsigned int j = 0; j < vCount; j++) {
			md.positions.push_back(l_positions[j]);
			md.normals.push_back(l_normals[j]);
			md.texCoords.push_back(l_texCoords[j]);
		}
		for (unsigned int j = 0; j < iCount; j++) {
			md.indices.push_back(l_indices[j]);
		}

		md.materials.push_back(render::material::brickwall());

		NodeData node0;
		node0.meshes.push_back(0);
		node0.tx = glm::mat4(1.0);
		md.nodes.push_back(node0);

		return Model(md);
	}

	Model ModelLoader::Wall(FACE face) {
		glm::vec3 normal;
		glm::vec3 p0, p1, p2, p3;
		float x = 0;
		float z = 0;
		// half side
		float distance = 10.0f;
		switch (face)
		{
		case comp::ModelLoader::LEFT:
			x = -distance;
			normal = glm::vec3(-1, 0, 0);
			p0 = { x, +distance, +distance}; p1 = { x, +distance,-distance }; p2 = {x,-distance,-distance }; p3 = {x,-distance,+distance };
			break;
		case comp::ModelLoader::RIGHT:
			x = distance;
			normal = glm::vec3(1, 0, 0);
			p0 = { x, +distance, -distance }; p1 = { x, +distance,+distance }; p2 = { x,-distance,+distance }; p3 = { x,-distance,-distance };
			break;
		case comp::ModelLoader::FRONT:
			z = distance;
			normal = glm::vec3(0, +1, 0);
			p0 = {+distance,+distance,z}; p1 = {-distance,+distance,z}; p2 = {-distance,-distance,z}; p3 = {+distance,-distance,z};
			break;
		case comp::ModelLoader::BACK:
			z = -distance;
			normal = glm::vec3(0, -1, 0);
			p0 = { -distance,+distance,z }; p1 = { +distance,+distance,z }; p2 = { +distance,-distance,z }; p3 = { -distance,-distance,z };
			break;
		default:
			assert(false && "invalid face enum");
		}
		const glm::vec3 l_positions[] = {
			p0 + glm::vec3(0,distance,0),
			p1 + glm::vec3(0,distance,0),
			p2 + glm::vec3(0,distance,0),
			p3 + glm::vec3(0,distance,0),
		};

		const glm::vec3 l_normals[] = {
			normal,
			normal,
			normal,
			normal,
		};

		const glm::vec2 l_texCoords[] = {
			glm::vec2(0.0f,  0.0f),
			glm::vec2(1.0f,  0.0f),
			glm::vec2(1.0f,  1.0f),
			glm::vec2(0.0f,  1.0f),
		};

		const unsigned int l_indices[] = {
			0, 1, 2,
			0, 2, 3,
			0, 3, 2,
			0, 2, 1,
		};

		ModelData md;
		md.nodes.reserve(1);
		md.meshes.reserve(1);

		const auto vCount = 4;
		const auto iCount = 12;

		MeshData mesh0{};
		mesh0.materialIndex = 0;
		mesh0.noOfIndices = iCount;
		mesh0.baseVertex = 0;
		mesh0.baseIndex = 0;
		md.meshes.push_back(mesh0);

		md.positions.reserve(vCount);
		md.normals.reserve(vCount);
		md.texCoords.reserve(vCount);
		md.indices.reserve(iCount);

		const aiVector3D Zero(0.0f, 0.0f, 0.0f);
		for (unsigned int j = 0; j < vCount; j++) {
			md.positions.push_back(l_positions[j]);
			md.normals.push_back(l_normals[j]);
			md.texCoords.push_back(l_texCoords[j]);
		}
		for (unsigned int j = 0; j < iCount; j++) {
			md.indices.push_back(l_indices[j]);
		}

		md.materials.push_back(render::material::brickwall());

		NodeData node0;
		node0.meshes.push_back(0);
		node0.tx = glm::mat4(1.0);
		md.nodes.push_back(node0);

		return Model(md);
	}

	Model ModelLoader::Load(std::string directory, std::string filepath) {
		comp::ModelData md;
		std::string completeFilePath = directory + DELIMITER_STR + filepath;
		Assimp::Importer importer;
		LOG(INFO) << "Loading Model: " << completeFilePath << "\n";
		const aiScene* scene = importer.ReadFile(completeFilePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
		if (!scene) {
			auto msg = std::string(importer.GetErrorString());
			LOG(ERROR) << "Failed to load model: " << msg << "\n";
			throw std::runtime_error("failed to load the file " + msg);
		}
		LOG(INFO) << "nodes of scene " << scene->mName.C_Str() << "\n";
		printNode("", scene->mRootNode);

		// RC = running count
		unsigned int verticesRC = 0;
		unsigned int indicesRC = 0;

		md.meshes.reserve(scene->mNumMeshes);
		for (index_t i = 0; i < scene->mNumMeshes; i++) {
			unsigned int thisMeshIndicesCount = scene->mMeshes[i]->mNumFaces * 3;
			md.meshes.push_back(MeshData{
					.materialIndex = scene->mMeshes[i]->mMaterialIndex,
					.baseVertex = verticesRC,
					.baseIndex = indicesRC,
					.noOfIndices = thisMeshIndicesCount,
			});

			verticesRC += scene->mMeshes[i]->mNumVertices;
			indicesRC += thisMeshIndicesCount;
		}

		md.positions.reserve(verticesRC);
		md.normals.reserve(verticesRC);
		md.texCoords.reserve(verticesRC);
		md.indices.reserve(indicesRC);

		const aiVector3D Zero(0.0f, 0.0f, 0.0f);
		for (unsigned int k = 0; k < scene->mNumMeshes; k++) {
			const aiMesh* mesh = scene->mMeshes[k];
			const bool hasTexCoords = mesh->HasTextureCoords(0);
			if (!hasTexCoords) {
				md.meshes[k].materialIndex = 0;
			}
			for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
				md.positions.push_back(glm::vec3(
					mesh->mVertices[j].x,
					mesh->mVertices[j].y,
					mesh->mVertices[j].z
				));
				md.normals.push_back(glm::vec3(
					mesh->mNormals[j].x,
					mesh->mNormals[j].y,
					mesh->mNormals[j].z
				));
				const auto& t = hasTexCoords ? mesh->mTextureCoords[0][j] : Zero;
				md.texCoords.push_back(glm::vec2(t.x, t.y));
			}
			for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
				const auto& face = mesh->mFaces[j];
				assert(face.mNumIndices == 3);
				md.indices.push_back(face.mIndices[0]);
				md.indices.push_back(face.mIndices[1]);
				md.indices.push_back(face.mIndices[2]);
			}
		}

		md.materials.reserve(scene->mNumMaterials);
		for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
			const aiMaterial* material = scene->mMaterials[i];
			md.materials.push_back(render::Material(directory, material));
		}

		std::map<aiNode*, index_t> nodesToIdxMapping;
		addNodeDataToList(md.nodes, nodesToIdxMapping, scene->mRootNode);
		addChildrenToNodeData(md.nodes, nodesToIdxMapping, scene->mRootNode);

		return Model(md);
	}
}
