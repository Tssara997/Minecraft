#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "block.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Noise/PerlinNoise.h"
#include "Noise/SimpleNoise.h"

class Chunk {
public:
	std::vector<float> vertexBuffer;

	Chunk(short int xGlobalOffset, short int zGlobalOffset) : xGlobalOffset{ xGlobalOffset }, zGlobalOffset{ zGlobalOffset }, gpuUploaded{ false }, noiseGenerator{1} {
		generateChunkData();
		generateMesh();
	}

	void generateChunkData() {
		float freq = 0.02f;
		float freq2 = 0.01f;
		float freq3 = 0.2f;
		float offset2 = 50.3f;
		float offset3 = 100.7f;
		float amp = 0.8f;
		float amp2 = 0.1f;
		float amp3 = 0.1f;
		for (int x = 0; x < chunkSizeX; x++) {
			for (int z = 0; z < chunkSizeZ; z++) {
				float heightNoise = amp * perlinNoise::perlin2((x + xGlobalOffset * chunkSizeX) * freq, (z + zGlobalOffset * chunkSizeZ) * freq);
				heightNoise += amp2 * perlinNoise::perlin2((x + xGlobalOffset * chunkSizeX) * freq2 + offset2, (z + zGlobalOffset * chunkSizeZ) * freq2 + offset2);
				heightNoise += amp3 * perlinNoise::perlin2((x + xGlobalOffset * chunkSizeX) * freq3 + offset3, (z + zGlobalOffset * chunkSizeZ) * freq3 + offset3);
				for (int y = bottomY; y < topY; y++) {
					if (y < heightNoise * topY)
						chunk[x][y][z] = Block::BlockType::DIRT;
					else
						chunk[x][y][z] = Block::BlockType::AIR;

				}
			}
		}
	}

	void generateMesh() {
		for (int x = 0; x < chunkSizeX; x++) {
			for (int z = 0; z < chunkSizeZ; z++) {
				for (int y = bottomY; y < topY; y++) {
					if (chunk[x][y][z] == Block::BlockType::AIR)
						continue;
					if (x == 0 || chunk[x - 1][y][z] == Block::BlockType::AIR)
						Block::addVertices(vertexBuffer, chunk[x][y][z], Block::Face::LEFT, x + xGlobalOffset * chunkSizeX, y, z + zGlobalOffset * chunkSizeZ);
					if (x == chunkSizeX - 1 || chunk[x + 1][y][z] == Block::BlockType::AIR)
						Block::addVertices(vertexBuffer, chunk[x][y][z], Block::Face::RIGHT, x + xGlobalOffset * chunkSizeX, y, z + zGlobalOffset * chunkSizeZ);
					if (z == 0 || chunk[x][y][z - 1] == Block::BlockType::AIR)
						Block::addVertices(vertexBuffer, chunk[x][y][z], Block::Face::BACK, x + xGlobalOffset * chunkSizeX, y, z + zGlobalOffset * chunkSizeZ);
					if (z == chunkSizeZ - 1 || chunk[x][y][z + 1] == Block::BlockType::AIR)
						Block::addVertices(vertexBuffer, chunk[x][y][z], Block::Face::FRONT, x + xGlobalOffset * chunkSizeX, y, z + zGlobalOffset * chunkSizeZ);
					if (y == bottomY || chunk[x][y - 1][z] == Block::BlockType::AIR)
						Block::addVertices(vertexBuffer, chunk[x][y][z], Block::Face::BOTTOM, x + xGlobalOffset * chunkSizeX, y, z + zGlobalOffset * chunkSizeZ);
					if (y == topY - 1 || chunk[x][y + 1][z] == Block::BlockType::AIR)
						Block::addVertices(vertexBuffer, chunk[x][y][z], Block::Face::UP, x + xGlobalOffset * chunkSizeX, y, z + zGlobalOffset * chunkSizeZ);
				}
			}
		}
	}

	void uploadToGPU() {
		if (gpuUploaded) return;
		if (vertexBuffer.empty()) return;

		if (VBO) { glDeleteBuffers(1, &VBO); VBO = 0; }
		if (VAO) { glDeleteVertexArrays(1, &VAO); VAO = 0; }
		if (texture) { glDeleteTextures(1, &texture); texture = 0; }

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * vertexBuffer.size()), vertexBuffer.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// TODO texture atlas
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		int width, height, channels;
		unsigned char* data = stbi_load("graphics\\dirtblock.jpg", &width, &height, &channels, 0);
		if (!data) {
			std::cerr << "Failed to load dirt texture" << std::endl;
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		gpuUploaded = true;
	}

	void draw(GLFWwindow* window) {
		if (vertexBuffer.size() == 0) return;
		if (!gpuUploaded) {
			uploadToGPU();
			if (!gpuUploaded)return;
		}

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexBuffer.size() / 5));
		glBindVertexArray(0);
	}

	void changeBlock(int x, int y, int z, Block::BlockType block) {
		// Implementation for changing a block in the chunk goes here
	}

	void deleteBlock(int x, int y, int z) {
		// Implementation for deleting a block in the chunk goes here
	}

	void changeMesh(int x, int y, int z, Block::BlockType block = Block::BlockType::AIR) {
		// Implementation for updating the chunk mesh goes here
	}

	void clearMesh() {
		vertexBuffer.clear();
	}

	short int getChunkPositonX() const {
		return xGlobalOffset;
	}

	short int getChunkPositonZ() const {
		return zGlobalOffset;
	}

private:
	int renderDistance;
	Block::BlockType chunk[16][256][16];
	const int bottomY = 0;
	const int topY = 86;
	const int chunkSizeX = 16;
	const int chunkSizeZ = 16;
	uint32_t VAO = 0;
	uint32_t VBO = 0;
	uint32_t texture = 0;
	short int xGlobalOffset;
	short int zGlobalOffset;
	bool gpuUploaded;
	SimplexNoise noiseGenerator;
};