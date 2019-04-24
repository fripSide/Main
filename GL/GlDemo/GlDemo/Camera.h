#pragma once
#include <glm/glm.hpp>
namespace GLDemo {
	enum CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera {

	public:
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectMatrix();
		glm::mat4 Perspective(float fov, float aspect, float near, float far);
		glm::mat4 Orthographic(float left, float right, float bottom, float top, float zNear, float zFar);
		glm::mat4 lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up);

		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		void ProcessKeyboard(CameraMovement direction, float deltaTime);

	private:
		void updateCameraVectors();

	public:
		glm::vec3 position_;
		glm::vec3 world_up_;
		glm::vec3 up_;
		glm::vec3 front_;
		glm::vec3 right_;
		glm::mat4 projection_mat_;


		float fov_;
		float aspect_;
		float near_;
		float far_;
		bool  is_perspective;

		// Euler Angles
		float yaw_ = -90.f;
		float pitch_ = 0.f;
		// Camera options
		float move_speed_ = 10.f;
		float mouse_sensitive_ = 0.08f;
		float zoom_ = 45.0f;

	private:
		
	};
}