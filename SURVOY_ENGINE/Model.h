#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include "Shader.h"
#include "Mesh.h"
#include <iostream>

namespace BAE {
	class Model
	{
	public:
		Model(std::string const &path);
		void Draw(Shader& shader);

	private:
		std::vector<Mesh> _meshes;
		std::string _directory;
		std::vector<TextureM> _textures_loaded;

		void _loadModel(std::string const& path);
		void _processNode(aiNode* node, const aiScene* scene);
		Mesh _processMesh(aiMesh* mesh, const aiScene* scene);
		unsigned int _textureFromFile(const char* path, const std::string& directory);
		std::vector<TextureM> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	};
}
