#include "SceneObjects.h"

//void Plane::

using namespace GLDemo;

QuadMesh::QuadMesh() {
	Positions.clear();
	Positions.emplace_back(-0.5f, 0.0f, 0.0f);
	Positions.emplace_back(0.5f, 0.0f, 0.0f);
	Positions.emplace_back(0.0f, 0.5f, 0.0f);
}

Plane::Plane() {
	mesh = new QuadMesh;
}