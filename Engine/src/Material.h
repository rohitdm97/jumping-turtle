#pragma once

#include <iostream>
#include <map>
#include <string>

#include "assimp/scene.h"

#include "Texture.h"

namespace render {
	class Shader;
	class Material;

	namespace material {
		enum TexType {
#define X(a) a,
#include "tex_types.enums"
#undef X
		};

		std::string Name(const TexType tt);
		std::ostream& operator<<(std::ostream& os, TexType tt);

		render::Material whiteTile();
	}

	class Material {
	private:
		std::map<material::TexType, render::Texture> ts;
	public:
		Material() = default;
		Material(const std::string& directory, const aiMaterial* material);

		void Add(material::TexType tt, const Texture& t);
		void Activate(const std::string& name, Shader&) const;
	};
}
