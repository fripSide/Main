#pragma once
#include "RenderObject.h"

namespace GLDemo {

	class Material;

	class QuadMesh : public Mesh {
	public:
		QuadMesh();
	};

	class ScreenMesh : public Mesh {
	public:
		ScreenMesh();
	};

	class CubeMesh : public Mesh {
	public:
		CubeMesh();
	};

	class TeapotMesh : public Mesh {
	public:
		TeapotMesh();
	};

	class SkyboxMesh : public Mesh {
	public:
		SkyboxMesh();
		void SetGL() override;
		void ResetGL() override;
	};
}
