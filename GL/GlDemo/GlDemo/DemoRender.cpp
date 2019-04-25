#include "DemoRender.h"
#include "RenderObject.h"
#include "SceneItems.h"

#include "glm/glm.hpp"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include <iostream>
#include "utils.h"
#include "Texture.h"

using namespace GLDemo;

void DemoRender::onSurfaceChanged(int width, int height) {
	glViewport(0, 0, width, height);
	World::Instance()->SetScreenSize(width, height);
}

const float M_PI = 3.14159265358979323846f;

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
	}; //[v1, v2, v3, v4] ������
	//trans = glm::translate(trans, glm::vec3(0.5, 0.5, 0));
	cb1->SetTransfrom(t1);
	//world.AddChildNode(cb1);

	auto cb2 = new SQUARE;
	glm::mat4 trans2;
	trans2 = glm::translate(trans2, glm::vec3(-0.5, -0.5, 0));
	cb2->SetTransfrom(trans2);
	//world.AddChildNode(cb2);

	// ����
	glm::mat4 s1 = {
		0.5, 0, 0, 0,
		0, 0.5, 0, 0,
		0, 0, 0.5, 0,
		0, 0, 0, 1
	};
	auto cbs = new SQUARE;
	cbs->SetTransfrom(s1);
	//world.AddChildNode(cbs);

	// ��ת
	float cos_90 = 0, sin_90 = 1;
	float cos_45 = sqrt(2) / 2, sin_45 = sqrt(2) / 2;
	glm::mat4 r1 = {
		1, 0, 0, 0,
		0, cos_45, sin_45, 0,
		0, -sin_45, cos_45, 0,
		0, 0, 0, 1,
	};
	glm::mat4 r2; // r2 == r1
	
	r2 = glm::rotate(r2, 45.0f / 180.0f * M_PI, glm::vec3(1, 0, 0));
	auto cbr = new SQUARE;
	cbr->SetTransfrom(r2);
	//world.AddChildNode(cbr);

	// s * r * t, ���ƣ���С 0.5, ��ת45��
	glm::mat4 cmbine = s1 * r1 * t1;
	auto cb = new SQUARE;
	cb->SetTransfrom(cmbine);
	world.AddChildNode(cb);
}

// ��֤����������ȷ��
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

/*
rotate ��һ�λ���һֱ��
*/
RenderObject *c = NULL;
void test_texture_mix(World & world) {
	auto p = new Plane;
	p->mtl_->SetTexture("texture1", Texture::LoadTexture("res/container.jpg"), 0);
	// OpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ��������Կ����������µߵ���
	p->mtl_->SetTexture("texture2", Texture::LoadTexture("res/awesomeface.png"), 1);
	// ��x����ת180�ȱ������
	//p->SetTransfrom(glm::rotate(glm::mat4(), M_PI, {1, 0, 0}));
	//world.AddChildNode(p);

	// cube�����ͼ
	c = new Cube;
	c->mtl_->SetTexture("texture1", Texture::LoadTexture("res/container.jpg"), 0);
	c->mtl_->SetTexture("texture2", Texture::LoadTexture("res/awesomeface.png"), 1);
	glm::mat4 tran;
	tran = glm::translate(glm::mat4(), { -0.5f, 0.3f, -10.f });
	tran = glm::rotate(tran, (GLfloat)glfwGetTime() * 0.1f, glm::vec3(0.0f, 0.1f, 0.0f));
	c->SetTransfrom(tran);
	world.AddChildNode(c);

	//world.AddChildNode(c);
	tran = glm::scale(glm::mat4(), {0.1, 0.1, 0.1});
	auto t = new Teapot;
	t->mtl_->SetTexture("texture1", Texture::LoadTexture("res/container.jpg"), 0);
	t->mtl_->SetTexture("texture2", Texture::LoadTexture("res/awesomeface.png"), 1);
	//tran = glm::translate(tran, { 0, 0, -10.f });
	t->SetTransfrom(tran);
	world.AddChildNode(t);

	SkyBox *sk = new SkyBox;
	world.AddChildNode(sk);

}

void test_post_effect_blur(World & world) {
	c = new RenderObject;
	c->mesh_ = new CubeMesh;
	c->mtl_ = new ShaderOnlyMaterial("shader/motion_obj.vs", "shader/motion_obj.fs");
	c->mtl_->scene_uniform_names_ = { "vp" }; // 
	c->mtl_->GetShader()->setMat4("curTran", c->transform_);
	c->mtl_->GetShader()->setMat4("prevTran", c->prev_trans_);
	c->mtl_->SetTexture("texture1", Texture::LoadTexture("res/container.jpg"), 0);
	c->mtl_->SetTexture("texture2", Texture::LoadTexture("res/awesomeface.png"), 1);
	glm::mat4 tran;
	tran = glm::translate(glm::mat4(), { -0.5f, 0.3f, -10.f });
	tran = glm::rotate(tran, (GLfloat)glfwGetTime() * 0.1f, glm::vec3(0.0f, 0.1f, 0.0f));
	c->SetTransfrom(tran);
	world.AddChildNode(c);

	auto t = new Teapot;
	t->mtl_->SetTexture("texture1", Texture::LoadTexture("res/container.jpg"), 0);
	t->mtl_->SetTexture("texture2", Texture::LoadTexture("res/awesomeface.png"), 1);
	tran = glm::scale(glm::mat4(), { 0.1, 0.1, 0.1 });
	t->SetTransfrom(tran);
	world.AddChildNode(t);

	SkyBox *sk = new SkyBox;
	world.AddChildNode(sk);
}

void DemoRender::onKeyEvent(int key_code) {
	//Log("key_code %d\n", key_code);
	CameraMovement dir = FORWARD;
	if (key_code == GLFW_KEY_W || key_code == GLFW_KEY_UP) {
		dir = FORWARD;
	}

	if (key_code == GLFW_KEY_A || key_code == GLFW_KEY_LEFT) {
		dir = LEFT;
	}

	if (key_code == GLFW_KEY_S || key_code == GLFW_KEY_DOWN) {
		dir = BACKWARD;
	}

	if (key_code == GLFW_KEY_D || key_code == GLFW_KEY_RIGHT) {
		dir = RIGHT;
	}
	world->mainCamera_.ProcessKeyboard(dir, delta_time);
}

void DemoRender::onMouseEvent(double xpos, double ypos) {
	double xOffset = xpos - lastX_;
	double yOffset = lastY_ - ypos;
	lastX_ = xpos;
	lastY_ = ypos;
	world->mainCamera_.ProcessMouseMovement(xOffset, yOffset);
}

/*
TODO:
1. ֧����ͼ��done�����������ͼ������������ת�Ƕȣ������任�÷��鶯������
ͶӰ�任 ����ͶӰ�任֮ǰ�����ξ���͸�ӳ�����������������Ļ���з����仯��(�Ƶ�)
FPS��� (������ƶ�)
TODO: ��һ���ع�����Scene������֯Ϊ����map<int, obj>�Ļ���id�Ĺ�������ֱ�ӹ���ָ�롣
*/
void DemoRender::onSurfaceCreated() {
	world = World::Instance();
	// �������λ�ã�
	world->mainCamera_.Perspective(glm::radians(45.0f), 4 / 3.f, 0.1f, 100.f);
	world->SetScreenSize(800, 600);
	// �����任
	//test_baisc_transfrom(world);
	//test_camera(world);
	//test_texture_mix(*world);
	test_post_effect_blur(*world);
}

float x = 0.0f, delta = 0.001f;
float len = 0.01f;
int dir = 1;
void DemoRender::onDrawFrame() {
	float current_ti = glfwGetTime();
	delta_time = current_ti - last_frame_ti;
	last_frame_ti = current_ti;
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 tran;
	x += delta * dir;
	if (x > len || x < -len) {
		dir = -dir;
	}
	tran = glm::translate(glm::mat4(), { x, x, 0.f });
	tran = glm::rotate(tran, 0.1f, glm::vec3(0.0f, 0.1f, 1.0f));

	if (c != NULL) {
		c->SetTransfrom(tran);
		c->mtl_->GetShader()->use();
		c->mtl_->GetShader()->setMat4("curTran", c->transform_);
		c->mtl_->GetShader()->setMat4("prevTran", c->prev_trans_);
	}
	

	// 
	world->Update();
	// render update
}