#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.hpp"
#include <stb_image.h>

class Texture {
public:
    GLuint id;
    int width, height, channels;
    std::string path;

    // Constructor: Load the texture from a file
    Texture(const std::string& filePath);

    // Bind the texture for drawing
    void bind(GLenum textureUnit) const;

    void bind() const;


    // Unbind the texture
    void unbind() const;

    // Destructor: free GPU texture resources
    ~Texture();
};

#endif // TEXTURE_H