#pragma once
#include "RenderObject.h"
#include "assimp/scene.h"

namespace GLDemo {
	struct AssimpVertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};
	class SkinnedMesh : public Mesh {
	public:
		SkinnedMesh(const std::vector<AssimpVertex> &vertex, std::vector<unsigned int> indices, std::vector<Texture> textures);

		std::vector<Texture> textures;
	};

	class SkinnedModel : public RenderObject {
	public:
		SkinnedModel(const char *path);

		void Clear();

		bool LoadMesh(const char *path);

	public:
		const aiScene *assimpScene_ = NULL;
		std::vector<SkinnedMesh> meshes_;

	private:
		void DoDraw() override;
		void processNode(aiNode *node, const aiScene *scene);
		SkinnedMesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mtl, aiTextureType ttype, const std::string &typeName);
	};
}