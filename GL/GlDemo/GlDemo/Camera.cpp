#include "Camera.h"

using namespace GLDemo;

// TODO1, �����任�� https://blog.csdn.net/wangdingqiaoit/article/details/51531002
// TODO2, �����ʵ�֣�https://blog.csdn.net/wangdingqiaoit/article/details/51570001


Camera::Camera(glm::vec3 eye, glm::vec3 up): 
	position_(eye), up_(up), front_(glm::vec3(0.0f, 1.0f, 0.0f)) {
}

glm::mat4 Camera::GetViewMatrix() {
	return lookAt(position_, glm::vec3(0, 0, 0), up_);
}

glm::mat4 Camera::lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 viewUp) {
	// �ӱ任�������������Ϊ��z�ᣬ��upΪy�ᣬ�������Ķ����任���������ϵ
	/*�Ƶ� https://blog.csdn.net/wangdingqiaoit/article/details/51394238
	OpenGLΪ��������ϵ���泯��Ļ���Ϸ�ΪY�ᣬ�ҷ�ΪX�ᣬ��ΪZ�ᡣ
	�������ϵ������Ϊ��[��1, ��2, ��3, 0] ��Ϊ�� [��1, ��2, ��3, 1]
	��������ϵ�Ļ���W = [x, y, z, P0] xyz��������(1, 0, 0, 0) (0, 1, 0, 0) (0, 0, 1, 0) P0ԭ��(0, 0, 0, 1)
		������꣺[x, y, z, P0] * [a1, a2, a3, 1]T
	�������ϵ�Ļ���[r, u, -f, E] ruf��������(rx, ry, rz, 0) (ux, uy, uz, 0) (-fx, -fy, -fz, 0) E�ӵ�(ex, ey, ez, 1)
	�������굽�������ı任����C = [r, u, -f, E]
	W * P1 = C * P2, W = I
	��P2 = C-1 * P1
	�������������е���� ��������������󣬼��ɻ���������ϵ�µ����ꡣ
	��������任���ӱ任������Ϊ��
	|-               -| -1
	| rx, ux, -fx, Ex |
	| ry, uy, -fy, Ey | 
	| rz, uz, -fz, Ez |
	| 0,  0,   0,  1  |
	|-               -|
	�������Ϊ��
	|A  B|-1 = |A-1 -(A-1)B(D-1)|
	|0  D|     | 0  D-1         |
	= 
	| rx,  ry,  rz,   -rTE |
	| ux,  uy,  uz,   -uTE |
	| -fx, -fy, -fz,  fTE  |
	| 0,   0,   0,    1    |
	*/
	glm::vec3 f = glm::normalize(target - eye);
	// ע�⣺ Cross(A, B) != Cross(B, A) ,�����ֶ���֪,   Cross(A, B) = -Cross(B, A) = |A|*|B|*Sin0
	glm::vec3 r = glm::normalize(glm::cross(f, viewUp));
	glm::vec3 u = glm::normalize(glm::cross(r, f)); // f,r,u��Ҫ����
	glm::mat4 view = {
		glm::vec4(r.x, u.x, -f.x, 0),
		glm::vec4(r.y, u.y, -f.y, 0),
		glm::vec4(r.z, u.z, -f.z, 0),
		glm::vec4(-glm::dot(r, eye), -glm::dot(u, eye), glm::dot(f, eye), 1),
	};
	return view;
}


glm::mat4 Camera::Perspective(float fov, float aspect, float near, float far) {
	return glm::mat4();
}

glm::mat4 Camera::Orthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
	return glm::mat4();
}