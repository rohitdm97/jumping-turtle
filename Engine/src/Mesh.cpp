#include "Mesh.h"
#include "log.h"

#include <glad/glad.h>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Shader.h"
#include "Texture.h"

constexpr unsigned int DIFFUSE_SLOT = GL_TEXTURE0;

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
	 glm::vec2(0.0f,  0.0f),  // bottom left
	 glm::vec2(0.0f,  0.0f),  // top 
};

unsigned int g_indices[] = {
	0, 1, 2
};

namespace comp {
	Mesh::Mesh() {
	}

	Mesh::Mesh(MeshData& md) {
		auto oldOldMD = &md;
		this->md = std::move(md);

		auto oldMD = &md;
		auto newMD = &this->md;

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

	Mesh::~Mesh() {
		glDeleteBuffers(NO_OF_BUFFERS, vbos);
		glDeleteVertexArrays(1, &VAO);
	}

	Mesh::Mesh(Mesh&& o) noexcept {
		this->md.meshes.swap(o.md.meshes);
		this->md.textures.swap(o.md.textures);

		this->md.indices.swap(o.md.indices);
		this->md.positions.swap(o.md.positions);
		this->md.normals.swap(o.md.normals);
		this->md.texCoords.swap(o.md.texCoords);

		this->VAO = o.VAO;
		o.VAO = 0;
		for (unsigned int i = 0; i < NO_OF_BUFFERS; i++) {
			this->vbos[i] = o.vbos[i];
			o.vbos[i] = 0;
		}
	}

	Mesh& Mesh::operator=(Mesh&& o) noexcept {
		this->md.meshes.swap(o.md.meshes);
		this->md.textures.swap(o.md.textures);

		this->md.indices.swap(o.md.indices);
		this->md.positions.swap(o.md.positions);
		this->md.normals.swap(o.md.normals);
		this->md.texCoords.swap(o.md.texCoords);

		this->VAO = o.VAO;
		o.VAO = 0;
		for (unsigned int i = 0; i < NO_OF_BUFFERS; i++) {
			this->vbos[i] = o.vbos[i];
			o.vbos[i] = 0;
		}

		return *this;
	}

	void Mesh::Attach(render::Shader& shader) {
		glBindVertexArray(VAO);

		{
			glBindBuffer(GL_ARRAY_BUFFER, vbos[B_POSITION]);
			int p = glGetAttribLocation(shader.ID(), "Position");
			if (p == -1) {
				throw std::invalid_argument("invalid shader does not have attrib Position");
			}
			glVertexAttribPointer(p, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(p);
		}
		//{
		//	glBindBuffer(GL_ARRAY_BUFFER, vbos[B_NORMAL]);
		//	int p = glGetAttribLocation(shader.ID(), "Normal");
		//	if (p == -1) {
		//		throw std::invalid_argument("invalid shader does not have attrib Normal");
		//	}
		//	glVertexAttribPointer(p, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//	glEnableVertexAttribArray(p);
		//}
		//{
		//	glBindBuffer(GL_ARRAY_BUFFER, vbos[B_TEXCOORDS]);
		//	int p = glGetAttribLocation(shader.ID(), "TexCoord");
		//	if (p == -1) {
		//		throw std::invalid_argument("invalid shader does not have attrib TexCoord");
		//	}
		//	glVertexAttribPointer(p, 2, GL_FLOAT, GL_FALSE, 0, 0);
		//	glEnableVertexAttribArray(p);
		//}

		glBindVertexArray(0);
	}

	void Mesh::Render() {
		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < md.meshes.size(); i++) {
			unsigned int matIndex = md.meshes[i].materialIndex;
			if (matIndex) {
				assert(matIndex < md.textures.size());
				assert(md.textures[matIndex].IsValid());
				md.textures[matIndex].Bind(DIFFUSE_SLOT);
			}

			glDrawElementsBaseVertex(GL_TRIANGLES,
				md.meshes[i].numOfIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * md.meshes[i].baseIndex),
				md.meshes[i].baseVertex
			);
		}
	}

	Mesh MeshLoader::Load(std::string directory, std::string filepath) {
		MeshData md;
		std::string completeFilePath = directory + DELIMITER_STR + filepath;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(completeFilePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
		if (!scene) {
			throw std::runtime_error("failed to load the file " + std::string(importer.GetErrorString()));
		}

		md.meshes.resize(scene->mNumMeshes);

		// RC = running count
		unsigned int verticesRC = 0;
		unsigned int indicesRC = 0;

		for (int i = 0; i < scene->mNumMeshes; i++) {
			unsigned int thisMeshIndicesCount = scene->mMeshes[i]->mNumFaces * 3;
			md.meshes[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
			md.meshes[i].numOfIndices = thisMeshIndicesCount;
			md.meshes[i].baseVertex = verticesRC;
			md.meshes[i].baseIndex = indicesRC;

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
				const auto& t = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][j] : Zero;
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

		md.textures.resize(scene->mNumMaterials);
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
					md.textures[i] = tl.Load(fullpath);
				}
			}
		}

		return Mesh(md);
	}

	Mesh createMesh() {
		MeshData md;
		md.meshes.resize(1);

		// RC = running count
		unsigned int verticesRC = 0;
		unsigned int indicesRC = 0;

		md.meshes[0].materialIndex = 0;
		md.meshes[0].numOfIndices = 3;
		md.meshes[0].baseVertex = verticesRC;
		md.meshes[0].baseIndex = indicesRC;

		verticesRC += 3;
		indicesRC += 3;

		md.positions.reserve(verticesRC);
		md.normals.reserve(verticesRC);
		md.texCoords.reserve(verticesRC);
		md.indices.reserve(indicesRC);

		const aiVector3D Zero(0.0f, 0.0f, 0.0f);
		for (unsigned int j = 0; j < 3; j++) {
			md.positions.push_back(g_positions[j]);
			md.normals.push_back(g_normals[j]);
			md.texCoords.push_back(g_texCoords[j]);
		}
		md.indices.push_back(g_indices[0]);
		md.indices.push_back(g_indices[1]);
		md.indices.push_back(g_indices[2]);

		return Mesh(md);
	}
}
