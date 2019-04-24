#include "SceneItems.h"
#include "Meshes.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace GLDemo;

Plane::Plane() {
	mesh_ = new QuadMesh;
	mtl_ = new BaseLightMaterial();
}

Cube::Cube() {
	mesh_ = new CubeMesh;
	mtl_ = new BaseLightMaterial;
}


BaseLightMaterial::BaseLightMaterial() {
	shader_ = new Shader();
	shader_->SetShaders("shader/base_light.vs", "shader/base_light.fs");
}

ShaderOnlyMaterial::ShaderOnlyMaterial(const char * vs_path, const char * fs_path) {
	shader_ = new Shader();
	shader_->SetShaders(vs_path, fs_path);
}

ShaderOnlyMaterial::~ShaderOnlyMaterial() {
	delete shader_;
}

CanvasMaterial::CanvasMaterial() {
	shader_ = new Shader;
	shader_->SetShaders("shader/canvas.vs", "shader/canvas.fs");
}

ScreenQuad::ScreenQuad() {
	mesh_ = new ScreenMesh;
	SetTransfrom(glm::scale(glm::mat4(), { 2, 2, 1 }));
	mtl_ = new CanvasMaterial;
}


SkyBox::SkyBox(const std::vector<std::string> & faces) {
	assert(faces.size() == 6);
	mtl_ = new ShaderOnlyMaterial("shader/skybox.vs", "shader/skybox.fs");
	mesh_ = new SkyboxMesh;
	mtl_->SetCubeMap("skybox", CubeMap::LoadCubeMap(faces));
}

const std::vector<std::string> chlorine_bay = {
	"res/sk2/chlorine-bay_rt.tga",
	"res/sk2/chlorine-bay_lf.tga",
	"res/sk2/chlorine-bay_up.tga",
	"res/sk2/chlorine-bay_dn.tga",
	
	"res/sk2/chlorine-bay_bk.tga",
	"res/sk2/chlorine-bay_ft.tga",
};

SkyBox::SkyBox() {
	mtl_ = new ShaderOnlyMaterial("shader/skybox.vs", "shader/skybox.fs");
	mesh_ = new SkyboxMesh;
	std::vector<std::string> faces = {
		"res/sk1/right.jpg",
		"res/sk1/left.jpg",
		"res/sk1/top.jpg",
		"res/sk1/bottom.jpg",
		"res/sk1/front.jpg",
		"res/sk1/back.jpg",
	};
	mtl_->scene_uniform_names_ = { "sky_vp" };
	mtl_->SetCubeMap("skybox", CubeMap::LoadCubeMap(faces));
}