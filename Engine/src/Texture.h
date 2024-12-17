#pragma once

#include <string>

namespace render {

	constexpr unsigned int INVALID_ID = -1L >> 1;

	class Texture {
	private:
		unsigned int ID_ = INVALID_ID;
		int width, height, nrChannels;
	public:
		void Bind(unsigned int slot);

		bool IsValid() const;

		friend class TextureLoader;
	};

	class TextureLoader {
	public:
		Texture Load(const std::string& filepath);
	};
}
