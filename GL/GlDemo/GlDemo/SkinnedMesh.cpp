#include "SkinnedMesh.h"
#include "assimp/Importer.hpp"
#include <assimp/postprocess.h>
#include "SceneItems.h"
#include "Texture.h"

#include "utils.h"

using namespace GLDemo;

SkinnedMesh::SkinnedMesh(const std::vector<AssimpVertex> &vertex, std::vector<unsigned int> indices, 
	std::vector<Texture*> textures) {
	Indices = std::move(indices);
	textures_ = std::move(textures);
	for (const auto & val : vertex) {
		Positions.push_back(std::move(val.Position));
		UV.push_back(std::move(val.TexCoords));
		Normals.push_back(std::move(val.Normal));
	}
	Init();
}


void SkinnedModel::Clear() {
	for (auto & it : tex_cache_) {
		delete it.second;
	}
	tex_cache_.clear();
}

SkinnedModel::SkinnedModel(const char *path) {
	LoadMesh(path);
	mtl_ = new ShaderOnlyMaterial("shader/model_loading.vs", "shader/model_loading.fs");
}

bool SkinnedModel::LoadMesh(const char *path) {
	Assimp::Importer importer;
	assimpScene_ = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs 
		| aiProcess_JoinIdenticalVertices);
	if (!assimpScene_ || assimpScene_->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene_->mRootNode) {
		Log("ERROR::ASSIMP::  Path:%s Error:%s\n", path, importer.GetErrorString());
		return false;
	}
	std::string pt(path);
	directory_ = pt.substr(0, pt.find_last_of('/'));

	// Log("node child num: %d\n", assimpScene_->mRootNode->mNumChildren);
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
	std::vector<Texture*> textures;
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

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse
		std::vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular
		std::vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture*> normalMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture*> heightMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	
	// texture
	return SkinnedMesh(vertices, indices, textures);
}

std::vector<Texture*> SkinnedModel::loadMaterialTextures(aiMaterial *mtl, aiTextureType ttype, const std::string &typeName) {
	std::vector<Texture*> textures;
	for (int i = 0; i < mtl->GetTextureCount(ttype); i++) {
		aiString str;
		mtl->GetTexture(ttype, i, &str);
		std::string pt = directory_ + '/' + std::string(str.C_Str());
		auto has_load = false;
		auto t = tex_cache_.find(pt);
		if (t != tex_cache_.end()) {
			textures.push_back(t->second);
			has_load = true;
			
		}
		if (!has_load) {
			Texture* tex = Texture::LoadTexture(pt.data(), GL_TEXTURE_2D, GL_RGB);
			tex->tex_usage = ttype;
			tex->tag = typeName;
			textures.push_back(tex);
			tex_cache_[pt] = tex;
		}
	}
	return textures;
}

void SkinnedModel::DoDraw() {
	if (mtl_ == NULL) {
		return;
	}
	Shader *shader = mtl_->GetShader();
	shader->use();

	for (auto & mesh : meshes_) {
		// setup textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (int i = 0; i < mesh.textures_.size(); i++) {

			Texture* tex = mesh.textures_[i];
			std::string number;
			std::string name = tex->tag;
			if (name == "textrue_diffuse") {
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream

			mtl_->SetTexture(name + number, tex, i);
		}
		SetupUniforms();
		mesh.Draw();
	}
}