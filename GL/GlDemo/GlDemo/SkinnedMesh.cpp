#include "SkinnedMesh.h"
#include "assimp/Importer.hpp"
#include <assimp/postprocess.h>
#include "SceneItems.h"

#include "utils.h"

using namespace GLDemo;

SkinnedMesh::SkinnedMesh(const std::vector<AssimpVertex> &vertex, std::vector<unsigned int> indices, 
	std::vector<Texture> textures) {
	Indices = std::move(indices);
	textures = std::move(textures);
	for (const auto & val : vertex) {
		Positions.push_back(std::move(val.Position));
		UV.push_back(std::move(val.TexCoords));
		Normals.push_back(std::move(val.Normal));
	}
	Init();
}


void SkinnedModel::Clear() {

}

SkinnedModel::SkinnedModel(const char *path) {
	LoadMesh(path);
	mtl_ = new BaseLightMaterial;
}

bool SkinnedModel::LoadMesh(const char *path) {
	Assimp::Importer importer;
	assimpScene_ = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs 
		| aiProcess_JoinIdenticalVertices);
	if (!assimpScene_ || assimpScene_->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene_->mRootNode) {
		Log("ERROR::ASSIMP::  Path:%s Error:%s\n", path, importer.GetErrorString());
		return false;
	}

	Log("node child num: %d\n", assimpScene_->mRootNode->mNumChildren);
	processNode(assimpScene_->mRootNode, assimpScene_);
	return true;
}

void SkinnedModel::processNode(aiNode *node, const aiScene *scene) {
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes_.push_back(std::move(processMesh(mesh, scene)));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

SkinnedMesh SkinnedModel::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<AssimpVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	// ¶¥µã
	for (int i = 0; i < mesh->mNumVertices; i++) {
		AssimpVertex vertex;
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0]) {
			auto &vec2 = mesh->mTextureCoords[0][i];
			vertex.TexCoords = glm::vec2(vec2.x, vec2.y);
		}
		// tangent
		aiVector3D vec3;
		if (mesh->mTangents != NULL) {
			vec3 = mesh->mTangents[i];

			vertex.Tangent = glm::vec3(vec3.x, vec3.y, vec3.z);
		}

		// bitangent
		if (mesh->mBitangents) {
			vec3 = mesh->mBitangents[i];
			vertex.Bitangent = glm::vec3(vec3.x, vec3.y, vec3.z);
		}

		vertices.push_back(vertex);
	}
	// 
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		auto face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

	// texture

	return SkinnedMesh(vertices, indices, textures);
}

std::vector<Texture> SkinnedModel::loadMaterialTextures(aiMaterial *mtl, aiTextureType ttype, const std::string &typeName) {
	std::vector<Texture> textures;
	return textures;
}

void SkinnedModel::DoDraw() {
	if (mtl_ == NULL) {
		return;
	}
	Shader *shader = mtl_->GetShader();
	shader->use();
	SetupUniforms();
	for (auto & mesh : meshes_) {
		mesh.Draw();
	}
}