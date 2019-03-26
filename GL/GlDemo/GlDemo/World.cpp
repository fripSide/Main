#include "World.h"
#include "SceneObjects.h"

using namespace GLDemo;

World::World() {
	root = new RenderObject();
}

void World::AddChildNode(RenderObject *node) {
	root->AddChild(node);
}

void World::Update() {
	root->Draw();
}