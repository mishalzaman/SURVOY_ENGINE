#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include <iostream>
#include "Shader.h"
#include "Mesh.h"
#include "FileLoader.h"

namespace BAE {
	class Model
	{
	public:
		Model(std::string const &path);
		void Draw(Shader& shader);

		int NumVertices();
		std::vector<STexture> Vertices();

	private:
		std::vector<Mesh> _meshes;
		std::string _directory;
		std::vector<STexture> _textures_loaded;

		void _loadModel(std::string const& path);
		void _processNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform);
		Mesh _processMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transformation);
		std::vector<STexture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);

		glm::mat4 _convertToGLMMat4(const aiMatrix4x4& from);
	};
}
