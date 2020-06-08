#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{

public:
	//camera data 
	Camera();
	Camera(float x, float y, float z, float fov);
	//void MouseMoved( double xpos, double ypos);
	void CheckMouseMovement(GLFWwindow& window);
	void MoveFront(float elapsedTime, float velocity);
	void MoveBack(float elapsedTime, float velocity);
	void MoveRight(float elapsedTime, float velocity);
	void MoveLeft(float elapsedTime, float velocity);
	glm::mat4 GetCameraTranslationMatrix();
	glm::vec3& GetCameraPosition();
	glm::vec3 & GetCameraFront();
	void SetCameraPosition(float x, float y, float z);
private:
	glm::vec3 CameraFront{};
	glm::vec3 CameraPos{};
	glm::vec3 CameraUp{};
	double yaw{};
	double pitch{};
	double lastX{};
	double lastY{};
	bool init{};
};


#ifdef CAMERA_IMPLEMENTATION
Camera::Camera()
	:Camera(0.0f, 0.0f, 0.0f, 45.0f)
{
}

Camera::Camera(float x, float y, float z, float fov)
	: CameraPos{ x,y,z }
	, CameraFront{ 0, 0,-1.0f }
	, CameraUp{ 0, 1, 0 }
	, yaw{ -90 }
	, pitch{ 0.0f }
	, lastX{ x }
	, lastY{ y }
	, init{ true }
{

}

void Camera::CheckMouseMovement(GLFWwindow& window)
{
	double xpos{ 0 }, ypos{ 0 };
	glfwGetCursorPos(&window, &xpos, &ypos);
	if (!init)
	{

		double xoffset = xpos - lastX;
		double yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec<3, double> direction;
		direction.x = cos(glm::radians(yaw))* cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw))* cos(glm::radians(pitch));
		CameraFront = glm::normalize(direction);
	}
	init = false;
}

void Camera::MoveFront(float elapsedTime, float velocity)
{
	CameraPos += CameraFront * velocity*elapsedTime;
}

void Camera::MoveBack(float elapsedTime, float velocity)
{
	CameraPos -= CameraFront * velocity*elapsedTime;
}

void Camera::MoveRight(float elapsedTime, float velocity)
{
	CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * elapsedTime*velocity;
}

void Camera::MoveLeft(float elapsedTime, float velocity)
{
	CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * elapsedTime*velocity;
}

glm::mat4 Camera::GetCameraTranslationMatrix()
{
	return glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

glm::vec3& Camera::GetCameraPosition()
{
	return CameraPos;
}

glm::vec3& Camera::GetCameraFront()
{
	return CameraFront;
}

void Camera::SetCameraPosition(float x, float y, float z)
{
	CameraPos.x = x;
	CameraPos.y = y;
	CameraPos.z = z;
}
#endif