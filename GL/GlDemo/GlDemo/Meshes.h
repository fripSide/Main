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

	class TeapotMesh : public Mesh {
	public:
		TeapotMesh();
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

	class Teapot : public RenderObject {
	public:
		Teapot() {
			mesh_ = new TeapotMesh();
			mtl_ = new BaseLightMeterial();
		}
	};
}
