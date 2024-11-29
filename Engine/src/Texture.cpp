#include "Texture.h"

#include <iostream>
#include "stb_image.h"

#include <glad/glad.h>

namespace render {
    Texture TextureLoader::Load(std::string& filepath) {
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.width, t.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
