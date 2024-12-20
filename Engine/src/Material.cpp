#include "Material.h"
#include "types.h"
#include "log.h"

#include <glad/glad.h>

#include "Shader.h"
#include "Texture.h"

aiTextureType aiTextureTypeFromTT(render::material::TexType tt) {
	switch (tt) {
#define X(a, b) case render::material::a: return aiTextureType_##a;
#include "tex_types.enums"
#undef X
	default:
		throw std::invalid_argument(std::string("unknown TexType(") + std::to_string(((int)tt)) + ")");
	}
}

void printMaterials(const aiMaterial* material) {
	const aiTextureType types[] = {
		aiTextureType_NONE,
		aiTextureType_DIFFUSE,
		aiTextureType_SPECULAR,
		aiTextureType_AMBIENT,
		aiTextureType_EMISSIVE,
		aiTextureType_HEIGHT,
		aiTextureType_NORMALS,
		aiTextureType_SHININESS,
		aiTextureType_OPACITY,
		aiTextureType_DISPLACEMENT,
		aiTextureType_LIGHTMAP,
		aiTextureType_REFLECTION,
		aiTextureType_BASE_COLOR,
		aiTextureType_NORMAL_CAMERA,
		aiTextureType_EMISSION_COLOR,
		aiTextureType_METALNESS,
		aiTextureType_DIFFUSE_ROUGHNESS,
		aiTextureType_AMBIENT_OCCLUSION,
		aiTextureType_UNKNOWN,
		aiTextureType_SHEEN,
		aiTextureType_CLEARCOAT,
		aiTextureType_TRANSMISSION,
		aiTextureType_MAYA_BASE,
		aiTextureType_MAYA_SPECULAR,
		aiTextureType_MAYA_SPECULAR_COLOR,
		aiTextureType_MAYA_SPECULAR_ROUGHNESS,
	};
	const std::string names[] = {
		"NONE",
		"DIFFUSE",
		"SPECULAR",
		"AMBIENT",
		"EMISSIVE",
		"HEIGHT",
		"NORMALS",
		"SHININESS",
		"OPACITY",
		"DISPLACEMENT",
		"LIGHTMAP",
		"REFLECTION",
		"BASE_COLOR",
		"NORMAL_CAMERA",
		"EMISSION_COLOR",
		"METALNESS",
		"DIFFUSE_ROUGHNESS",
		"AMBIENT_OCCLUSION",
		"UNKNOWN",
		"SHEEN",
		"CLEARCOAT",
		"TRANSMISSION",
		"MAYA_BASE",
		"MAYA_SPECULAR",
		"MAYA_SPECULAR_COLOR",
		"MAYA_SPECULAR_ROUGHNESS",
	};
	const index_t length = sizeof(types) / sizeof(types[0]);
	for (index_t i = 0; i < length; i++) {
		const int count = material->GetTextureCount(types[i]);
		if (count > 0) {
			aiString path;
			const bool success = material->GetTexture(types[i], 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS;
			if (success) {
				LOG(DEBUG) << "found " << names[i] << " map (" << count << ")" << path.data << "\n";
			}
			else {
				LOG(DEBUG) << "did not find " << names[i] << " map(" << count << ")\n";
			}
		}
		else {
			LOG(TRACE) << "did not find " << names[i] << " map (" << count << ")\n";
		}
	}
}

std::string render::material::Name(const TexType tt) {
	switch (tt) {
#define X(a, b) case render::material::a: return #b;
#include "tex_types.enums"
#undef X
	default:
		throw std::invalid_argument(std::string("unknown TexType(") + std::to_string(((int)tt)) + ")");
	}
}

std::ostream& render::material::operator<<(std::ostream& os, TexType tt) {
	switch (tt) {
#define X(a, b) case render::material::a: os << #a; return os;
#include "tex_types.enums"
#undef X
	default:
		throw std::invalid_argument("unknown TexType " + ((int)tt));
	}
}

render::Material render::material::whiteTile() {
	render::Material m;
	render::TextureLoader tl;
	const auto texFile = std::string("..") + DELIMITER_STR + "assets" + DELIMITER_STR + "white_tile.png";

	m.Add(render::material::TexType::DIFFUSE, tl.Load(texFile));
	m.Add(render::material::TexType::SPECULAR, tl.Load(texFile));

	return m;
}

render::Material::Material(const std::string& directory, const aiMaterial* material) {
	render::TextureLoader tl;
	constexpr auto types_length = 3;
	const material::TexType types[types_length] = {
		material::AMBIENT,
		material::DIFFUSE,
		material::SPECULAR,
	};
	for (index_t i = 0; i < types_length; i++) {
		material::TexType tt = types[i];
		aiTextureType att = aiTextureTypeFromTT(tt);
		if (material->GetTextureCount(att) > 0) {
			aiString path;
			const bool success = material->GetTexture(att, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS;
			if (success) {
				std::string p(path.data);
				if (p.substr(0, 2) == ("." + DELIMITER_CHAR)) {
					p = p.substr(2, p.size() - 2);
				}
				std::string fullpath = directory + DELIMITER_CHAR + p;
				ts[tt] = tl.Load(fullpath);
			}
		}
	}

	printMaterials(material);
}

void render::Material::Add(material::TexType tt, const Texture& t) {
	ts[tt] = t;
}

void render::Material::Activate(const std::string& name, render::Shader& shader) const {
	for (auto& tPair : ts) {
		const auto tt = tPair.first;
		auto& t = tPair.second;
		t.Bind(GL_TEXTURE0 + tt);
		const std::string uname = name + "." + Name(tt);
		shader.Uniforms(false).SetInt(uname, tt);
	}
}
