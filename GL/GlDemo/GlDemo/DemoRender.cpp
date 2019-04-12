#include "DemoRender.h"
#include "RenderObject.h"
#include "SceneObjects.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include <iostream>
#include "utils.h"

using namespace GLDemo;

void DemoRender::onDrawFrame() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	world.Update();
}

void DemoRender::onSurfaceChanged(int width, int height) {

}

void test_baisc_transfrom(World & world) {
	// https://blog.csdn.net/wangdingqiaoit/article/details/51531002

#define SQUARE Plane
	// #define SQUARE Cube

	// transform
	auto cb1 = new SQUARE;
	glm::mat4 t1 = {
		1, 0, 0, 0, // v1
		0, 1, 0, 0, // v2
		0, 0, 1, 0, // v3
		0.5, 0.5, 0, 1 // v4
	}; //[v1, v2, v3, v4] 列向量
	//trans = glm::translate(trans, glm::vec3(0.5, 0.5, 0));
	cb1->SetTransfrom(t1);
	//world.AddChildNode(cb1);

	auto cb2 = new SQUARE;
	glm::mat4 trans2;
	trans2 = glm::translate(trans2, glm::vec3(-0.5, -0.5, 0));
	cb2->SetTransfrom(trans2);
	//world.AddChildNode(cb2);

	// 缩放
	glm::mat4 s1 = {
		0.5, 0, 0, 0,
		0, 0.5, 0, 0,
		0, 0, 0.5, 0,
		0, 0, 0, 1
	};
	auto cbs = new SQUARE;
	cbs->SetTransfrom(s1);
	//world.AddChildNode(cbs);

	// 旋转
	float cos_90 = 0, sin_90 = 1;
	float cos_45 = sqrt(2) / 2, sin_45 = sqrt(2) / 2;
	glm::mat4 r1 = {
		1, 0, 0, 0,
		0, cos_45, sin_45, 0,
		0, -sin_45, cos_45, 0,
		0, 0, 0, 1,
	};
	glm::mat4 r2; // r2 == r1
	const float M_PI = 3.14159265358979323846f;
	r2 = glm::rotate(r2, 45.0f / 180.0f * M_PI, glm::vec3(1, 0, 0));
	auto cbr = new SQUARE;
	cbr->SetTransfrom(r2);
	//world.AddChildNode(cbr);

	// s * r * t, 上移，缩小 0.5, 旋转45度
	glm::mat4 cmbine = s1 * r1 * t1;
	auto cb = new SQUARE;
	cb->SetTransfrom(cmbine);
	world.AddChildNode(cb);
}

void test_camera(World & world) {
	glm::vec3 eye(1, 0, 1);
	glm::vec3 up(0, 1, 0);
	Camera ca(eye, up);
	glm::mat4 view = ca.GetViewMatrix();
	glm::mat4 v2 = glm::lookAtRH(eye, glm::vec3(0, 0, 0), up);
	DumpMat4(view);
	DumpMat4(v2);
	glm::vec3 u3;
}

void DemoRender::onSurfaceCreated() {
	// 基础变换
	//test_baisc_transfrom(world);
	test_camera(world);

}