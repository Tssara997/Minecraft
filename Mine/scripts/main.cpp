//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
#include <atomic>
#include <memory>
#include <chrono>

#include "shaders/Shader_H.h"
#include "movment/Camera.h"
#include "worldRender/chunkManager.h"

const uint32_t SCR_WIDTH = 1000;
const uint32_t SCR_HEIGHT = 800;
Camera ourCamera;
float FOV = 45.0f;
std::atomic<bool> isRunning = true;
std::atomic<bool> chunkUpdateNeeded = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void calculateFPS();
void chunkUpdate(ChunkManager& worldChunk);


int main() {
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Minecraft", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		std::cerr << "Failed to create GLFW window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	 //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // waireframe mode

	// shaders
	Shader* ourShader = new Shader("scripts\\shaders\\vertexShader.glsl", "scripts\\shaders\\fragmentShader.glsl");

	// mouse settings
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// world chunk
	std::shared_ptr<ChunkManager> worldChunk = std::make_shared<ChunkManager>(8);

	std::thread chunkUpdateThread(chunkUpdate, std::ref(*worldChunk));
	//worldChunk->disableNewChunkGeneration();

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		calculateFPS();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader->use();

		glm::mat4 model = glm::mat4(1.0f);
		int modelLoc = glGetUniformLocation(ourShader->ID, "model");
		if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 view = ourCamera.getViewMatrix();
		glm::mat4 proj = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

		uint32_t viewLoc = glGetUniformLocation(ourShader->ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		uint32_t projLoc = glGetUniformLocation(ourShader->ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		worldChunk->draw(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	isRunning = false;
	chunkUpdateNeeded = false;
	chunkUpdateThread.join();
	glDeleteProgram(ourShader->ID);
	glfwTerminate();
	delete ourShader;
	return 0;
}

void chunkUpdate(ChunkManager& worldChunk) {
	while (isRunning) {
		while (chunkUpdateNeeded) {
			worldChunk.updateChunks(ourCamera.getCameraPosition());
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		isRunning = false;
		chunkUpdateNeeded = false;
	}
	ourCamera.moveCamera(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	ourCamera.mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	FOV -= float(yoffset);
	if (FOV < 1.0f)
		FOV = 1.0f;
	if (FOV > 90.0f)
		FOV = 90.0f;
}

void calculateFPS() {
	static int frameCount = 0;
	static double previousTime = 0.0;
	double currentTime = glfwGetTime();
	frameCount++;
	// If a second has passed.
	if (currentTime - previousTime >= 1.0)
	{
		// Display the frame count here any way you want.
		std::cout << "FPS: " << frameCount << std::endl;
		frameCount = 0;
		previousTime = currentTime;
	}
}