#include "SceneObjects.h"
#include "RenderObject.h"

//void Plane::

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
	Indices.insert(Indices.begin(), std::begin(indices), std::end(indices));
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
	m_Shader = new Shader();
	m_Shader->SetShaders("shader/base_light.vs", "shader/base_light.fs");
}