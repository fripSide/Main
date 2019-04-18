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

void World::AddChildNode(RenderObject *node) {
	root->AddChild(node);
}

void World::Update() {
	root->Draw();
}