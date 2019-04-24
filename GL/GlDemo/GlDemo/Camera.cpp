#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace GLDemo;

// TODO1, 基本变换， https://blog.csdn.net/wangdingqiaoit/article/details/51531002
// TODO2, 相机的实现，https://blog.csdn.net/wangdingqiaoit/article/details/51570001


Camera::Camera(glm::vec3 eye, glm::vec3 up): 
	position_(eye), up_(up), front_(glm::vec3(0.0f, 0.0f, -1.0f)) {
	world_up_ = glm::vec3(0.f, 1.f, 0.f);
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
	return lookAt(position_, position_ + front_, up_);
}

glm::mat4 Camera::lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 viewUp) {
	// 视变换，以相机正方向为负z轴，以up为y轴，将其他的东西变换到这个坐标系
	/*推导 https://blog.csdn.net/wangdingqiaoit/article/details/51394238
	OpenGL为右手坐标系，面朝屏幕，上方为Y轴，右方为X轴，后方为Z轴。
	齐次坐标系下向量为：[α1, α2, α3, 0] 点为： [α1, α2, α3, 1]
	世界坐标系的基：W = [x, y, z, P0] xyz轴向量：(1, 0, 0, 0) (0, 1, 0, 0) (0, 0, 1, 0) P0原点(0, 0, 0, 1)
		点的坐标：[x, y, z, P0] * [a1, a2, a3, 1]T
	相机坐标系的基：[r, u, -f, E] ruf轴向量：(rx, ry, rz, 0) (ux, uy, uz, 0) (-fx, -fy, -fz, 0) E视点(ex, ey, ez, 1)
	世界坐标到相机坐标的变换矩阵：C = [r, u, -f, E]
	W * P1 = C * P2, W = I
	则：P2 = C-1 * P1
	世界坐标下所有点乘以 相机基矩阵的逆矩阵，即可获得相机坐标系下的坐标。
	所以相机变换（视变换）矩阵为：
	| rx, ux, -fx, Ex |-1
	| ry, uy, -fy, Ey | 
	| rz, uz, -fz, Ez |
	| 0,  0,   0,  1  |
	其逆矩阵为：
	|A  B|-1 = |A-1 -(A-1)B(D-1)|
	|0  D|     | 0  D-1         |
	= 
	| rx,  ry,  rz,   -rTE |
	| ux,  uy,  uz,   -uTE |
	| -fx, -fy, -fz,  fTE  |
	| 0,   0,   0,    1    |
	*/
	glm::vec3 f = glm::normalize(target - eye);

	// 注意： Cross(A, B) != Cross(B, A) ,由右手定则知,   Cross(A, B) = -Cross(B, A) = |A|*|B|*Sin0
	glm::vec3 r = glm::normalize(glm::cross(f, viewUp));
	glm::vec3 u = glm::normalize(glm::cross(r, f)); // f,r,u需要正交
	glm::mat4 view = {
		glm::vec4(r.x, u.x, -f.x, 0),
		glm::vec4(r.y, u.y, -f.y, 0),
		glm::vec4(r.z, u.z, -f.z, 0),
		glm::vec4(-glm::dot(r, eye), -glm::dot(u, eye), glm::dot(f, eye), 1),
	};
	return view;
}

/*
https://blog.csdn.net/wangdingqiaoit/article/details/51589825#comments
fov为视角
aspect宽高比
h = near * tan(fov / 2) (矩形高度的一半)
w = h * aspect

*/
glm::mat4 Camera::Perspective(float fov, float aspect, float near, float far) {
	float tanHalfOfFov = tan(fov / 2.0f);

	glm::mat4 my = {
		glm::vec4(1 / (aspect * tanHalfOfFov), 0, 0, 0),
		glm::vec4(0, 1 / tanHalfOfFov, 0, 0),
		glm::vec4(0, 0, (far + near) / (near - far), 2 * far * near / (near - far)),
		glm::vec4(0, 0, -1, 0),
	};
	projection_mat_ = glm::perspective(fov, aspect, near, far);
	return projection_mat_;
}

glm::mat4 Camera::Orthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
	return glm::mat4();
}

glm::mat4 Camera::GetProjectMatrix() {
	return projection_mat_;
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = move_speed_ * deltaTime;
	if (direction == FORWARD)
		position_ += front_ * velocity;
	if (direction == BACKWARD)
		position_ -= front_ * velocity;
	if (direction == LEFT)
		position_ -= right_ * velocity;
	if (direction == RIGHT)
		position_ += right_ * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
	xoffset *= mouse_sensitive_;
	yoffset *= mouse_sensitive_;

	yaw_ += xoffset;
	pitch_ += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch_ > 89.0f)
			pitch_ = 89.0f;
		if (pitch_ < -89.0f)
			pitch_ = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

	front_ = glm::normalize(front);
	right_ = glm::normalize(glm::cross(front_, world_up_));
	up_ = glm::normalize(glm::cross(right_, front_));
}