#include "FileLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi_image.h"

void ENGINE::FileLoader::Texture(STexture& texture)
{
    std::string pathStr(texture.path);
    size_t dotPosition = pathStr.find_last_of(".");
    std::string extension = "";

    if (dotPosition != std::string::npos) {
        extension = pathStr.substr(dotPosition + 1);
    }

    // Adjust vertical flipping based on file type or other criteria if needed
    //bool shouldFlipVertically = /* condition based on `extension` or other logic */
    //stbi_set_flip_vertically_on_load(shouldFlipVertically);

    glGenTextures(1, &texture.id);

    unsigned char* data = stbi_load(texture.path.c_str(), &texture.width, &texture.height, &texture.channel, 0);
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

        stbi_image_free(data); // Free image data after uploading to GPU
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texture.path << std::endl;
    }
}


void ENGINE::FileLoader::TextureSkyBox(STexture& texture, const std::vector<std::string>& faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data); // This line can be removed as it's redundant because data is NULL in this case.
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    texture.id = textureID;
}
