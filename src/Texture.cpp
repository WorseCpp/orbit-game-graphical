#define STB_IMAGE_IMPLEMENTATION
#include "Texture.hpp"



// Constructor: Load the texture from a file
Texture::Texture(const std::string& filePath)
    : width(0), height(0), channels(0), path(filePath)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Set wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Flip the image vertically during load if required
    // stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (data) {
        GLenum format;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;
        else
            format = GL_RGB;  // Fallback

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Texture failed to load at path: " << filePath << std::endl;
    }
    stbi_image_free(data);
}

// Bind the texture for drawing
void Texture::bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::bind() const {
    bind(GL_TEXTURE0);
}

// Unbind the texture
void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Destructor: free GPU texture resources
Texture::~Texture() {
    glDeleteTextures(1, &id);
}
