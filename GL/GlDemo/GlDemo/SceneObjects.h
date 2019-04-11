#pragma once
#include "RenderObject.h"

namespace GLDemo {

	class Material;

	class QuadMesh : public Mesh {
	public:
		QuadMesh();
	};

	class CubeMesh : public Mesh {
	public:
		CubeMesh();
	};

	class Plane : public RenderObject {
	public:
		Plane();
	};

	class Cube : public RenderObject {
	public:
		Cube();
	};

	class BaseLightMeterial : public Meterial {
	public:
		BaseLightMeterial();
	};
}
