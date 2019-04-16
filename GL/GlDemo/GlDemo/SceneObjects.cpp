#include "SceneObjects.h"
#include "RenderObject.h"

//void Plane::

/*
TODO:
floor改成可设置网格密度
cube，添加uv和normal
添加ball。

*/

using namespace GLDemo;

QuadMesh::QuadMesh() {
	Positions.clear();
	Positions.emplace_back(-0.5f, 0.5f, 0.0f);
	Positions.emplace_back(0.5f, 0.5f, 0.0f);
	Positions.emplace_back(0.5f, -0.5f, 0.0f);
	Positions.emplace_back(-0.5f, -0.5f, 0.0f);

	Indices.clear();
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2,  // first Triangle
		2, 3, 0   // second Triangle
	};
	unsigned int uvs[] = {
		0.f, 1.f,   // top left
		1.0f, 1.0f, //T right
		1.f, 0.f, // bottom right
		0.f, 0.f, // bottom left
	};
	UV.emplace_back(0.f, 1.f);
	UV.emplace_back(1.0f, 1.0f);
	UV.emplace_back(1.f, 0.f);
	UV.emplace_back(0.f, 0.f);
	Indices.insert(Indices.end(), std::begin(indices), std::end(indices));
	
	Init();
}

CubeMesh::CubeMesh() {
	Positions.clear();
	Positions.emplace_back(-0.5, -0.5, -0.5);
	Positions.emplace_back(-0.5f, -0.5f, -0.5f);
	Positions.emplace_back(0.5f, 0.5f, -0.5f);
	Positions.emplace_back(0.5f, -0.5f, -0.5f),
	Positions.emplace_back(0.5f, 0.5f, -0.5f),
	Positions.emplace_back(-0.5f, -0.5f, -0.5f),
	Positions.emplace_back(-0.5f, 0.5f, -0.5f),

	Positions.emplace_back(-0.5f, -0.5f, 0.5f),
	Positions.emplace_back(0.5f, -0.5f, 0.5f),
	Positions.emplace_back(0.5f, 0.5f, 0.5f),
	Positions.emplace_back(0.5f, 0.5f, 0.5f),
	Positions.emplace_back(-0.5f, 0.5f, 0.5f),
	Positions.emplace_back(-0.5f, -0.5f, 0.5f),

	Positions.emplace_back(-0.5f, 0.5f, 0.5f),
	Positions.emplace_back(-0.5f, 0.5f, -0.5f),
	Positions.emplace_back(-0.5f, -0.5f, -0.5f),
	Positions.emplace_back(-0.5f, -0.5f, -0.5f),
	Positions.emplace_back(-0.5f, -0.5f, 0.5f),
	Positions.emplace_back(-0.5f, 0.5f, 0.5f),

	Positions.emplace_back(0.5f, 0.5f, 0.5f),
	Positions.emplace_back(0.5f, -0.5f, -0.5f),
	Positions.emplace_back(0.5f, 0.5f, -0.5f),
	Positions.emplace_back(0.5f, -0.5f, -0.5f),
	Positions.emplace_back(0.5f, 0.5f, 0.5f),
	Positions.emplace_back(0.5f, -0.5f, 0.5f),

	Positions.emplace_back(-0.5f, -0.5f, -0.5f),
	Positions.emplace_back(0.5f, -0.5f, -0.5f),
	Positions.emplace_back(0.5f, -0.5f, 0.5f),
	Positions.emplace_back(0.5f, -0.5f, 0.5f),
	Positions.emplace_back(-0.5f, -0.5f, 0.5f),
	Positions.emplace_back(-0.5f, -0.5f, -0.5f),

	Positions.emplace_back(-0.5f, 0.5f, -0.5f),
	Positions.emplace_back(0.5f, 0.5f, 0.5f),
	Positions.emplace_back(0.5f, 0.5f, -0.5f),
	Positions.emplace_back(0.5f, 0.5f, 0.5f),
	Positions.emplace_back(-0.5f, 0.5f, -0.5f),
	Positions.emplace_back(-0.5f, 0.5f, 0.5f);

	// uv
	UV = std::vector<glm::vec2>{
		  glm::vec2(0.0f, 0.0f),
		  glm::vec2(1.0f, 1.0f),
		  glm::vec2(1.0f, 0.0f),
		  glm::vec2(1.0f, 1.0f),
		  glm::vec2(0.0f, 0.0f),
		  glm::vec2(0.0f, 1.0f),

		  glm::vec2(0.0f, 0.0f),
		  glm::vec2(1.0f, 0.0f),
		  glm::vec2(1.0f, 1.0f),
		  glm::vec2(1.0f, 1.0f),
		  glm::vec2(0.0f, 1.0f),
		  glm::vec2(0.0f, 0.0f),

		  glm::vec2(1.0f, 0.0f),
		  glm::vec2(1.0f, 1.0f),
		  glm::vec2(0.0f, 1.0f),
		  glm::vec2(0.0f, 1.0f),
		  glm::vec2(0.0f, 0.0f),
		  glm::vec2(1.0f, 0.0f),

		  glm::vec2(1.0f, 0.0f),
		  glm::vec2(0.0f, 1.0f),
		  glm::vec2(1.0f, 1.0f),
		  glm::vec2(0.0f, 1.0f),
		  glm::vec2(1.0f, 0.0f),
		  glm::vec2(0.0f, 0.0f),

		  glm::vec2(0.0f, 1.0f),
		  glm::vec2(1.0f, 1.0f),
		  glm::vec2(1.0f, 0.0f),
		  glm::vec2(1.0f, 0.0f),
		  glm::vec2(0.0f, 0.0f),
		  glm::vec2(0.0f, 1.0f),

		  glm::vec2(0.0f, 1.0f),
		  glm::vec2(1.0f, 0.0f),
		  glm::vec2(1.0f, 1.0f),
		  glm::vec2(1.0f, 0.0f),
		  glm::vec2(0.0f, 1.0f),
		  glm::vec2(0.0f, 0.0f),
	};

	Init();
}


Plane::Plane() {
	mesh_ = new QuadMesh;
	mtl_ = new BaseLightMeterial();
}

Cube::Cube() {
	mesh_ = new CubeMesh;
	mtl_ = new BaseLightMeterial;
}

BaseLightMeterial::BaseLightMeterial() {
	shader_ = new Shader();
	shader_->SetShaders("shader/base_light.vs", "shader/base_light.fs");
}