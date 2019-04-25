#pragma once
#include "RenderObject.h"
#include "Meshes.h"

namespace GLDemo {

	class Plane : public RenderObject {
	public:
		Plane();
	};

	class Cube : public RenderObject {
	public:
		Cube();
	};

	class BaseLightMaterial : public Material {
	public:
		BaseLightMaterial();
	};

	class ShaderOnlyMaterial : public Material {
	public:
		ShaderOnlyMaterial(const char * vs_path = "shader/base_light.vs", const char * fs_path = "shader/base_light.fs");
		~ShaderOnlyMaterial();
	};

	class CanvasMaterial : public Material {
	public:
		CanvasMaterial();
	};

	class ScreenQuad : public RenderObject {
	public:
		ScreenQuad();
	};

	class Teapot : public RenderObject {
	public:
		Teapot() {
			mesh_ = new TeapotMesh();
			mtl_ = new BaseLightMaterial();
		}
	};

	// http://www.custommapmakers.org/skyboxes.php
	class SkyBox : public RenderObject {
	public:
		SkyBox(const std::vector<std::string> & faces);
		SkyBox();
	};

}