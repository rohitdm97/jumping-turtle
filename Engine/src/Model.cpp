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
#include "Texture.h"

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from) {
	glm::mat4 to;

	to[0][0] = from->a1; to[0][1] = from->b1;  to[0][2] = from->c1; to[0][3] = from->d1;
	to[1][0] = from->a2; to[1][1] = from->b2;  to[1][2] = from->c2; to[1][3] = from->d2;
	to[2][0] = from->a3; to[2][1] = from->b3;  to[2][2] = from->c3; to[2][3] = from->d3;
	to[3][0] = from->a4; to[3][1] = from->b4;  to[3][2] = from->c4; to[3][3] = from->d4;

	return to;
}

glm::vec3 g_positions[] = {
	glm::vec3(0.5f, -0.5f, 0.0f),  // bottom right
	glm::vec3(-0.5f, -0.5f, 0.0f),  // bottom left
	glm::vec3(0.0f,  0.5f, 0.0f),  // top 
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
	0, 1, 2,
	0, 2, 1,
};

namespace comp {
	Model createModel() {
		ModelData md;
		md.nodes.reserve(1);
		md.meshes.reserve(1);

		MeshData mesh0;
		mesh0.materialIndex = 0;
		mesh0.noOfIndices = 6;
		mesh0.baseVertex = 0;
		mesh0.baseIndex = 0;
		md.meshes.push_back(mesh0);

		md.positions.reserve(3);
		md.normals.reserve(3);
		md.texCoords.reserve(3);
		md.indices.reserve(6);

		const aiVector3D Zero(0.0f, 0.0f, 0.0f);
		for (unsigned int j = 0; j < 3; j++) {
			md.positions.push_back(g_positions[j]);
			md.normals.push_back(g_normals[j]);
			md.texCoords.push_back(g_texCoords[j]);
		}
		for (unsigned int j = 0; j < 6; j++) {
			md.indices.push_back(g_indices[j]);
		}

		render::TextureLoader tl;
		const auto texFile = std::string("..") + DELIMITER_STR + "assets" + DELIMITER_STR + "white_tile.png";
		md.textures.push_back(tl.Load(texFile));

		NodeData node0;
		node0.meshes.push_back(0);
		node0.tx = glm::mat4(1.0);
		md.nodes.push_back(node0);

		return Model(md);
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
		for (index i = 0; i < node->mNumMeshes; i++) {
			std::cout << node->mMeshes[i] << ", ";
		}
		std::cout << "]\n";
	}
	std::cout << prefix << "children count: " << node->mNumChildren << "\n";
	for (index i = 0; i < node->mNumChildren; i++) {
		printNode(prefix + "--", node->mChildren[i]);
	}
}

namespace comp {
	void Model::calculateMatrix() {
		this->matrixCached = position * rotation * (scale * glm::mat4(1.0f));
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
		glBindBuffer(GL_ARRAY_BUFFER, vbos[B_NORMAL]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(this->md.normals[0]) * this->md.normals.size(), &this->md.normals[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[B_TEXCOORDS]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(this->md.texCoords[0]) * this->md.texCoords.size(), &this->md.texCoords[0], GL_STATIC_DRAW);

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

	Model::Model(Model&& o) {
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

	Model& Model::operator=(Model&& o) {
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

	void Model::SetPosition(glm::vec3 position) {
		this->position = glm::translate(glm::mat4(1.0f), position);
		this->calculateMatrix();
	}
	void Model::Rotate(glm::vec3 axis, float angleInRadians) {
		this->rotation = glm::rotate(this->rotation, angleInRadians, axis);
		this->calculateMatrix();
	}

	void Model::Attach(render::Shader& shader) {
		glBindVertexArray(VAO);
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbos[B_POSITION]);
			int p = glGetAttribLocation(shader.ID(), "Position");
			assert(p != -1 && "expects Position attrib used by the shader");
			glVertexAttribPointer(p, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(p);
		}
		//{
		//	glBindBuffer(GL_ARRAY_BUFFER, vbos[B_NORMAL]);
		//	int p = glGetAttribLocation(shader.ID(), "Normal");
		//	assert(p != -1 && "expects Normal attrib used by the shader");
		//	glVertexAttribPointer(p, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//	glEnableVertexAttribArray(p);
		//}
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbos[B_TEXCOORDS]);
			int p = glGetAttribLocation(shader.ID(), "TexCoord");
			assert(p != -1 && "expects TexCoord attrib used by the shader");
			glVertexAttribPointer(p, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(p);
		}
		glBindVertexArray(0);
	}

	void Model::renderNode(render::Shader& shader, const glm::mat4& parentTx, const index n) {
		const glm::mat4 thisTx = parentTx * md.nodes[n].tx;
		shader.SetMat4("Mesh", thisTx);

		// draw call
		for (const index mIdx : md.nodes[n].meshes) {
			// todo activate texture
			const auto tIdx = md.meshes[mIdx].materialIndex;
			if (tIdx < md.textures.size()) {
				assert(tIdx >= 0 && "materialIndex is negative");
				assert(tIdx < md.textures.size() && "invalid materialIndex");
				assert(md.textures[tIdx].IsValid() && "invalid texture, texture loading has failed");
				md.textures[tIdx].Bind(GL_TEXTURE0);
			}

			glDrawElementsBaseVertex(GL_TRIANGLES,
				md.meshes[mIdx].noOfIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * md.meshes[mIdx].baseIndex),
				md.meshes[mIdx].baseVertex
			);
		}

		for (const index cIdx : md.nodes[n].children) {
			renderNode(shader, thisTx, cIdx);
		}
	}

	void Model::Render(render::Shader& shader) {
		glBindVertexArray(VAO);
		shader.SetMat4("Model", matrixCached);
		shader.SetInt("diffuse", 0);
		renderNode(shader, glm::mat4(1.0f), 0);
		glBindVertexArray(0);
	}

	void addNodeDataToList(std::vector<NodeData>& result, std::map<aiNode*, index>& nodesToIdxMapping, aiNode* node) {
		if (node == nullptr) {
			return;
		}
		const index newIndex = result.size();
		NodeData nd;
		nd.tx = aiMatrix4x4ToGlm(&node->mTransformation);
		nd.meshes.reserve(node->mNumMeshes);
		for (index i = 0; i < node->mNumMeshes; i++) {
			nd.meshes.push_back(node->mMeshes[i]);
		}
		result.push_back(nd);
		nodesToIdxMapping[node] = newIndex;
		if (node->mChildren == nullptr) {
			return;
		}
		for (index i = 0; i < node->mNumChildren; i++) {
			addNodeDataToList(result, nodesToIdxMapping, node->mChildren[i]);
		}
	}

	void addChildrenToNodeData(std::vector<NodeData>& nodes, std::map<aiNode*, index>& nodesToIdxMapping, aiNode* node) {
		if (node == nullptr) {
			return;
		}
		if (node->mChildren == nullptr) {
			return;
		}
		assert(nodesToIdxMapping.find(node) != nodesToIdxMapping.end());
		const index idx = nodesToIdxMapping[node];
		NodeData& nd = nodes[idx];
		nd.children.reserve(node->mNumChildren);
		for (index i = 0; i < node->mNumChildren; i++) {
			assert(nodesToIdxMapping.find(node->mChildren[i]) != nodesToIdxMapping.end());
			const index cIdx = nodesToIdxMapping[node->mChildren[i]];
			nd.children.push_back(cIdx);
		}
		for (index i = 0; i < node->mNumChildren; i++) {
			addChildrenToNodeData(nodes, nodesToIdxMapping, node->mChildren[i]);
		}
	}

	Model ModelLoader::Load(std::string directory, std::string filepath) {
		comp::ModelData md;
		std::string completeFilePath = directory + DELIMITER_STR + filepath;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(completeFilePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
		LOG(INFO) << "nodes of scene " << scene->mName.C_Str() << "\n";
		printNode("", scene->mRootNode);
		if (!scene) {
			throw std::runtime_error("failed to load the file " + std::string(importer.GetErrorString()));
		}

		// RC = running count
		unsigned int verticesRC = 0;
		unsigned int indicesRC = 0;

		md.meshes.reserve(scene->mNumMeshes);
		for (index i = 0; i < scene->mNumMeshes; i++) {
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

		md.textures.reserve(scene->mNumMaterials);
		render::TextureLoader tl;

		for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
			const aiMaterial* material = scene->mMaterials[i];
			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
				aiString path;
				const bool success = material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS;
				if (success) {
					std::string p(path.data);
					if (p.substr(0, 2) == ("." + DELIMITER_CHAR)) {
						p = p.substr(2, p.size() - 2);
					}
					std::string fullpath = directory + DELIMITER_CHAR + p;
					md.textures.push_back(tl.Load(fullpath));
				}
			}
		}

		std::map<aiNode*, index> nodesToIdxMapping;
		addNodeDataToList(md.nodes, nodesToIdxMapping, scene->mRootNode);
		addChildrenToNodeData(md.nodes, nodesToIdxMapping, scene->mRootNode);

		return Model(md);
	}
}
