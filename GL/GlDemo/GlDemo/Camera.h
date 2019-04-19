#pragma once
#include <glm/glm.hpp>
namespace GLDemo {
	class Camera {

	public:
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectMatrix();
		glm::mat4 Perspective(float fov, float aspect, float near, float far);
		glm::mat4 Orthographic(float left, float right, float bottom, float top, float zNear, float zFar);
		glm::mat4 lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up);

	public:
		glm::vec3 position_;
		glm::vec3 up_;
		glm::vec3 front_;
		glm::mat4 projection_mat_;

		float fov_;
		float aspect_;
		float near_;
		float far_;
		bool  is_perspective;

	private:
		
	};
}