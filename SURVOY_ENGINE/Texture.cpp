#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi_image.h"

BAE::Texture::Texture(const std::string& bitmapFont):
    _textureId(0),
    _width(0),
    _height(0),
    _channels(1)
{
    _import(_textureId, bitmapFont, _width, _height, _channels);
}

BAE::Texture::~Texture()
{
}

bool BAE::Texture::Load(GLuint& fontTexture, const std::string& bitmapFont, int& width, int& height, int& channels)
{
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Disable mipmapping


    // for bitmap font
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // load and generate the texture
    unsigned char* data = stbi_load(bitmapFont.c_str(), &width, &height, &channels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
            std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
            return false; 
    }
    stbi_image_free(data);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void BAE::Texture::_import(GLuint& fontTexture, const std::string& bitmapFont, int& width, int& height, int& channels)
{
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Disable mipmapping


    // for bitmap font
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // load and generate the texture
    unsigned char* data = stbi_load(bitmapFont.c_str(), &width, &height, &channels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(data);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}
