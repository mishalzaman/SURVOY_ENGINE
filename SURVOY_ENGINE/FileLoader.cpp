#include "FileLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi_image.h"

void BAE::FileLoader::Texture(GLuint& textureID, const std::string& path, int& width, int& height, int& channels)
{
    std::string pathStr(path);
    size_t dotPosition = pathStr.find_last_of(".");
    std::string extension = "";

    if (dotPosition != std::string::npos) {
        extension = pathStr.substr(dotPosition + 1);
    }

    if (extension != "png") {
        stbi_set_flip_vertically_on_load(true); // for obj file 
    }

    std::string filename = std::string(path);

    glGenTextures(1, &textureID);

    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (data)
    {
        GLenum format{};
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}

void BAE::FileLoader::Texture(STexture& texture)
{
    std::string pathStr(texture.path);
    size_t dotPosition = pathStr.find_last_of(".");
    std::string extension = "";

    if (dotPosition != std::string::npos) {
        extension = pathStr.substr(dotPosition + 1);
    }

    if (extension != "png") {
        stbi_set_flip_vertically_on_load(true); // for obj file 
    }

    std::string filename = std::string(texture.path);

    glGenTextures(1, &texture.id);

    unsigned char* data = stbi_load(filename.c_str(), &texture.width, &texture.height, &texture.channel, 0);
    if (data)
    {
        GLenum format{};
        if (texture.channel == 1)
            format = GL_RED;
        else if (texture.channel == 3)
            format = GL_RGB;
        else if (texture.channel == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texture.path << std::endl;
        stbi_image_free(data);
    }
}
