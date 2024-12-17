#include "Texture.h"
#include "log.h"

#include <iostream>
#include "stb_image.h"

#include <glad/glad.h>

int extractType(int nrChannels) noexcept {
    switch (nrChannels) {
    case 3:
        return GL_RGB;
    case 4:
        return GL_RGBA;
    default:
        throw std::runtime_error("invalid no of channels expected 3/4");
    }
}

namespace render {
    Texture TextureLoader::Load(const std::string& filepath) {
        LOG(DEBUG) << "Loading Texture " << filepath << "\n";
        Texture t;
        unsigned char* data = stbi_load(filepath.c_str(), &t.width, &t.height, &t.nrChannels, 0);
        if (!data) {
            throw std::runtime_error("failed to load file " + filepath);
        }
        glGenTextures(1, &t.ID_);
        glBindTexture(GL_TEXTURE_2D, t.ID_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        const auto type = extractType(t.nrChannels);
        glTexImage2D(GL_TEXTURE_2D, 0, type, t.width, t.height, 0, type, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return t;
    }

    void Texture::Bind(unsigned int slot) {
        glActiveTexture(slot);
        glBindTexture(GL_TEXTURE_2D, ID_);
    }
    bool Texture::IsValid() const {
        // INVALID_ID ^ INVALID_ID ==> 0
        // anything else ^ INVALID_ID ==> >0
        return ID_ ^ INVALID_ID;
    }
}
