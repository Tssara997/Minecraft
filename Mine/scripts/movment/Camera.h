#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera {
private:
	glm::mat4 view;
	glm::vec3 cameraPos = glm::vec3(0.0f, 52.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	const float cameraSpeed = 10.0f;
	const float sensitivity = 0.001f;
	float yaw = 45;
	float pitch = 0.0;
	bool firstMouse = true;
	int lastX, lastY;

public:
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		if (firstMouse)
		{
			glfwGetWindowSize(window, &lastX, &lastY);
			lastX /= 2;
			lastY /= 2;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = ypos - lastY;
		lastX = xpos;
		lastY = ypos;

		xoffset *= 2 * sensitivity;
		yoffset *= -sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 1.57f)
			pitch = 1.57f;
		if (pitch < -1.565f)
			pitch = -1.565f;

		glm::vec3 direction{
			cos(yaw) * cos(pitch),
			sin(pitch),
			sin(yaw) * cos(pitch),
		};

		cameraFront = glm::normalize(direction);
	}

	Camera() = default;

	void moveCamera(GLFWwindow* window) {
		// input
		static float lastTime = glfwGetTime();
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * deltaTime * cameraFront;

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * deltaTime * cameraFront;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraUp * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraUp * deltaTime;


		lastTime = currentTime;

		view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
	}

	glm::mat4 getViewMatrix() {
		return view;
	}

	glm::vec3 getCameraPosition() {
		return cameraPos;
	}
};