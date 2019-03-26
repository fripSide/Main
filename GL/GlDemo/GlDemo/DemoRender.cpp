#include "DemoRender.h"
#include "RenderObject.h"
#include "SceneObjects.h"

using namespace GLDemo;

void DemoRender::onDrawFrame() {
	world.Update();
}

void DemoRender::onSurfaceChanged(int width, int height) {

}

void DemoRender::onSurfaceCreated() {
	world.AddChildNode(new Plane);
}