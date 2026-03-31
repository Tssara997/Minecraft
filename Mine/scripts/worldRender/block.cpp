#include "block.h"

#include<iostream>

Block::BlockType Block::currentBlockVertices = BlockType::AIR;
std::array<float, 180> Block::vertices = {
	0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	// front face
	1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	// left face
	0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	// right face
	1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	// bottom face
	1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	// top face
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

const std::map<Block::BlockType, BlockData> Block::blockTypes = {
		{ BlockType::AIR, BlockData({-1, -1}, {-1, -1}, {-1, -1})},
		{ BlockType::DIRT, BlockData({2, 0}, {2, 0}, {2, 0})},
		{ BlockType::GRASSBLOCK, BlockData({0, 0}, {3, 0}, {2, 0})},
		{ BlockType::STONE, BlockData({1, 0}, {1, 0}, {1, 0})},
		{ BlockType::OAK_LOG, BlockData({5, 1}, {4, 1}, {5, 1})},
		{ BlockType::OAK_PLANKS, BlockData({4, 0}, {4, 0}, {4, 0})}
};

void Block::addVertices(std::vector<float>& vertexBuffer, BlockType block, Face face, int xOffset, int yOffset, int zOffset) {
	//TODO use texture atlas UVs based on block data
	if (block == BlockType::AIR) return;

	if (currentBlockVertices != block) {
		createVerticesForBlock(block);
		currentBlockVertices = block;
	}

	BlockData data = blockTypes.at(block);
	for (int i = 0; i < 6; i++) {
		vertexBuffer.push_back(vertices[faceVertices * (int)face + faceLength * i] + xOffset);
		vertexBuffer.push_back(vertices[faceVertices * (int)face + faceLength * i + 1] + yOffset);
		vertexBuffer.push_back(vertices[faceVertices * (int)face + faceLength * i + 2] + zOffset);
		vertexBuffer.push_back(vertices[faceVertices * (int)face + faceLength * i + 3]);
		vertexBuffer.push_back(vertices[faceVertices * (int)face + faceLength * i + 4]);
	}
}

void Block::createVerticesForBlock(BlockType block) {
	BlockData coords = blockTypes.at(block);

	vertices = {
		// back face
		0.0f, 0.0f, 0.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		1.0f, 0.0f, 0.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		1.0f, 1.0f, 0.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		1.0f, 1.0f, 0.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		0.0f, 1.0f, 0.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		0.0f, 0.0f, 0.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		// front face
		1.0f, 1.0f, 1.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		1.0f, 0.0f, 1.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		0.0f, 0.0f, 1.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		0.0f, 0.0f, 1.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		0.0f, 1.0f, 1.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		1.0f, 1.0f, 1.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		// left face
		0.0f, 0.0f, 0.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		0.0f, 1.0f, 0.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		0.0f, 1.0f, 1.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		0.0f, 1.0f, 1.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		0.0f, 0.0f, 1.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		0.0f, 0.0f, 0.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		// right face
		1.0f, 1.0f, 1.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		1.0f, 1.0f, 0.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		1.0f, 0.0f, 0.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		1.0f, 0.0f, 0.0f, (coords.sideTex[0] + 0.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		1.0f, 0.0f, 1.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 1.0f) * tileSize,
		1.0f, 1.0f, 1.0f, (coords.sideTex[0] + 1.0f) * tileSize, (coords.sideTex[1] + 0.0f) * tileSize,
		// bottom face
		1.0f, 0.0f, 1.0f, (coords.botTex[0] + 1.0f) * tileSize, (coords.botTex[1] + 0.0f) * tileSize,
		1.0f, 0.0f, 0.0f, (coords.botTex[0] + 1.0f) * tileSize, (coords.botTex[1] + 1.0f) * tileSize,
		0.0f, 0.0f, 0.0f, (coords.botTex[0] + 0.0f) * tileSize, (coords.botTex[1] + 1.0f) * tileSize,
		0.0f, 0.0f, 0.0f, (coords.botTex[0] + 0.0f) * tileSize, (coords.botTex[1] + 1.0f) * tileSize,
		0.0f, 0.0f, 1.0f, (coords.botTex[0] + 0.0f) * tileSize, (coords.botTex[1] + 0.0f) * tileSize,
		1.0f, 0.0f, 1.0f, (coords.botTex[0] + 1.0f) * tileSize, (coords.botTex[1] + 0.0f) * tileSize,
		// top face
		0.0f, 1.0f, 0.0f, (coords.topTex[0] + 0.0f) * tileSize, (coords.topTex[1] + 1.0f) * tileSize,
		1.0f, 1.0f, 0.0f, (coords.topTex[0] + 1.0f) * tileSize, (coords.topTex[1] + 1.0f) * tileSize,
		1.0f, 1.0f, 1.0f, (coords.topTex[0] + 1.0f) * tileSize, (coords.topTex[1] + 0.0f) * tileSize,
		1.0f, 1.0f, 1.0f, (coords.topTex[0] + 1.0f) * tileSize, (coords.topTex[1] + 0.0f) * tileSize,
		0.0f, 1.0f, 1.0f, (coords.topTex[0] + 0.0f) * tileSize, (coords.topTex[1] + 0.0f) * tileSize,
		0.0f, 1.0f, 0.0f, (coords.topTex[0] + 0.0f) * tileSize, (coords.topTex[1] + 1.0f) * tileSize
	};
}