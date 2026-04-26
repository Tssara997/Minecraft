#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "block.h"

struct Biome_Data {
	float freq[3];
	float amp[3];
	float offset[3];
	float sinkess;
	float chance;
	std::string name;
};

class Chunk {
public:
	std::vector<float> vertexBuffer;

	Chunk(int xGlobalOffset, int zGlobalOffset);
	void generateChunkData();
	double getNoiseheight(int x, int z, Biome_Data biom) const;
	void generateMesh();
	void uploadToGPU();
	void draw(GLFWwindow* window);

	void clearMesh();
	short int getChunkPositonX() const;
	short int getChunkPositonZ() const;

	// TODO implement block changes without regenerating whole mesh
	bool isBlock(int x, int y, int z);
	void changeBlock(int x, int y, int z, Block::BlockType block);
	void deleteBlock(int x, int y, int z);
	void changeMesh(int x, int y, int z, Block::BlockType block = Block::BlockType::AIR);

private:
	int renderDistance;

	Block::BlockType chunk[16][256][16];
	const int bottomY = 0;
	const int topY = 86;
	const int chunkSizeX = 16;
	const int chunkSizeZ = 16;
	int xGlobalOffset;
	int zGlobalOffset;

	uint32_t VAO = 0;
	uint32_t VBO = 0;
	uint32_t texture = 0;
	bool gpuUploaded;

	Biome_Data highlands;
	Biome_Data midlands;
	Biome_Data lowlands;
};