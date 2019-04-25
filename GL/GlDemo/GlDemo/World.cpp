#include "World.h"
#include "RenderObject.h"

using namespace GLDemo;

World *World::instance_ = NULL;

World::World() {
	root = new RenderObject();
}

World::~World() {
	delete root;
}


World* World::Instance() {
	if (instance_ == NULL) {
		instance_ = new World();
	}
	return instance_;
}

RenderObject* World::GetAtiveScene() {
	return root;
}

void World::AddChildNode(RenderObject *node) {
	root->AddChild(node);
}
void World::SetScreenSize(int w, int h) {
	render_pass_.SetScreenSize(w, h);
	mainCamera_.Perspective(glm::radians(45.0f), w / h, 0.1f, 100.f);
}

void World::Update() {
	// update transform

	// update render
	render_pass_.RenderPipeline();
}

void World::SetSceneUniforms(RenderObject *node) {
	auto names = node->mtl_->GetUnifromNames();
	for (auto n : names) {
		SetUnifromByName(node, n);
	}
	node->mtl_->SetupUniforms();
}

void World::SetUnifromByName(RenderObject *node, const std::string &name) {
	if (name == "mvp") {
		auto v = mainCamera_.GetViewMatrix();
		auto p = mainCamera_.GetProjectMatrix();
		node->mtl_->GetShader()->setMat4(name, p * v * node->GetTransfrom());
	}
	else if (name == "vp") {
		auto v = mainCamera_.GetViewMatrix();
		auto p = mainCamera_.GetProjectMatrix();
		node->mtl_->GetShader()->setMat4(name, p * v);
	}
	else if (name == "sky_vp") {
		auto v = glm::mat4(glm::mat3(mainCamera_.GetViewMatrix()));
		auto p = mainCamera_.GetProjectMatrix();
		node->mtl_->GetShader()->setMat4(name, p * v);
	}
}