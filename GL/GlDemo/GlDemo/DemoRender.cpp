#include "DemoRender.h"
#include "RenderObject.h"
#include "SceneObjects.h"
#include "glm/glm.hpp"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace GLDemo;

void DemoRender::onDrawFrame() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	world.Update();
}

void DemoRender::onSurfaceChanged(int width, int height) {

}

void DemoRender::onSurfaceCreated() {
	//world.AddChildNode(new Plane);
	auto cb = new Cube;
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(0.2, 0.5, 0.2));
	cb->SetTransfrom(trans);
	Camera ca(glm::vec3(1), glm::vec3(3));
	auto t = ca.lookAt(glm::vec3(1), glm::vec3(3), glm::vec3(1));
	world.AddChildNode(cb);
}