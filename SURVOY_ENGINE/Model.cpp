#include "Model.h"
#include <iostream>


/*
IMPORTANT NOTES

- Blender
  - When exporting make sure to triangulate faces otherwise a range error is thrown
  - Check scaling
  - Creating Parent-Child relationship. Select the child, select the parent. Then press ctrl-p and select "Object (Keep transforms)"
*/

using namespace BAE;

BAE::Model::Model(std::string const& path)
{
    _loadModel(path);
}

void BAE::Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < _meshes.size(); i++)
		_meshes[i].Draw(shader);
}

int BAE::Model::NumVertices()
{
    int count = 0;

    for (unsigned int i = 0; i < _meshes.size(); i++)
        count += _meshes[i].NumVertices();

    return count;
}

void BAE::Model::_loadModel(std::string const& path)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    _directory = path.substr(0, path.find_last_of('/'));

    _processNode(scene->mRootNode, scene, glm::mat4(1.0f));
}

void BAE::Model::_processNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform)
{
    // apply rotation of -45 if FBX file. Otherwise comment this out for a OBJ file.
    parentTransform = glm::rotate(parentTransform, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // Convert aiMatrix4x4 to glm::mat4
    glm::mat4 nodeTransform = _convertToGLMMat4(node->mTransformation);

    // Combine with parent transform
    glm::mat4 globalTransform = parentTransform * nodeTransform;

    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(_processMesh(mesh, scene, globalTransform));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        _processNode(node->mChildren[i], scene, globalTransform);

    }
}

Mesh BAE::Model::_processMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transformation)
{
    // data to fill
    std::vector<SVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<STexture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        SVertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            //// tangent
            //vector.x = mesh->mTangents[i].x;
            //vector.y = mesh->mTangents[i].y;
            //vector.z = mesh->mTangents[i].z;
            //vertex.Tangent = vector;
            //// bitangent
            //vector.x = mesh->mBitangents[i].x;
            //vector.y = mesh->mBitangents[i].y;
            //vector.z = mesh->mBitangents[i].z;
            //vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<STexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<STexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<STexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<STexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    //glm::mat4 scaledTransformation = glm::scale(transformation, glm::vec3(0.2f, 0.2f, 0.2f));

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures, transformation);
}

std::vector<STexture> BAE::Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
{
    std::vector<STexture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);
        std::string fullPath = _directory + '/' + str.C_Str();
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < _textures_loaded.size(); j++)
        {
            if (std::strcmp(_textures_loaded[j].path.data(), fullPath.c_str()) == 0)
            {
                textures.push_back(_textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            STexture texture;
            texture.path = fullPath;
            
            BAE::FileLoader::Texture(texture);

            textures.push_back(texture);

            _textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}

glm::mat4 BAE::Model::_convertToGLMMat4(const aiMatrix4x4& from)
{
    glm::mat4 to;

    // Copy the data from aiMatrix4x4 to glm::mat4
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;

    return to;
}
