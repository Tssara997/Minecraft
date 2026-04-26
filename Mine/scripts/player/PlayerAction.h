#pragma once
#include "Camera.h"
#include "..\scripts\worldRender\chunkManager.h"

class PlayerAction {
private:
	float reach;
	ChunkManager* chunkManager;
	glm::vec3 blockInSight;
	bool wasPressed = false;
public:
	PlayerAction(ChunkManager* world, float max_reach) : chunkManager{ world }, reach{ max_reach } {}

	bool isBlockInSight(Camera &player) {
		auto origin = player.getCameraPosition();
		auto direction = player.getCameraFront();
		for (float i{}; i <= reach; i += 0.1f) {
			auto checkPos = origin + direction * i;
			if (chunkManager->isBlock(checkPos)) {
				std::cout << "hit" << std::endl;
				blockInSight = checkPos;
				return true;
			}
		}
		return false;
	}

	void handleInput(GLFWwindow* window, Camera &player) {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !wasPressed) {
			if (isBlockInSight(player)) {
				deleteBlock(window);
			}
			wasPressed = true;
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			wasPressed = false;
		}
	}

	void deleteBlock(GLFWwindow* window) {
		chunkManager->deleteBlock(blockInSight);
	}

	void changeBlock();

	void placeBlock();
};