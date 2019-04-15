#include "Camera.h"

using namespace GLDemo;

// TODO1, �����任�� https://blog.csdn.net/wangdingqiaoit/article/details/51531002
// TODO2, �����ʵ�֣�https://blog.csdn.net/wangdingqiaoit/article/details/51570001


Camera::Camera(glm::vec3 position, glm::vec3 up): 
	position_(position), up_(up), front_(glm::vec3(0.0f, 1.0f, 0.0f)) {
}

glm::mat4 Camera::GetViewMatrix() {
	
	return glm::mat4();
}

glm::mat4 Camera::lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 viewUp) {
	// �ӱ任�������������Ϊ��z�ᣬ��upΪy�ᣬ�������Ķ����任���������ϵ
	glm::vec3 f = glm::normalize(target - eye);
	glm::vec3 r = glm::normalize(glm::cross(f, viewUp));
	glm::vec3 u = glm::normalize(glm::cross(r, f));
	return glm::mat4();
}


glm::mat4 Camera::Perspective(float fov, float aspect, float near, float far) {
	return glm::mat4();
}

glm::mat4 Camera::Orthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
	return glm::mat4();
}