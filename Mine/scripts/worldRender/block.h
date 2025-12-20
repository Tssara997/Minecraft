#pragma once
#include <map>
#include <vector>

struct BlockData {
	int topTex;
	int sideTex;
	int botTex;
};

class Block {
public:
	enum class BlockType { AIR, DIRT, GRASSBLOCK, STONE, OAK_LOG, OAK_PLANKS };
	enum class Face { BACK, FRONT, LEFT, RIGHT, BOTTOM, UP };

	static void addVertices(std::vector<float>& vertexBuffer, BlockType block, Face face, int xOffset, int yOffset, int zOffset) {
		//TODO use texture atlas UVs based on block data
		if (block == BlockType::AIR) return;

		BlockData data = blockTypes.at(block);
		for (int i = 0; i < 6; i++) {
			vertexBuffer.push_back(vertices[faceVertices * (int)face + faceLength * i] + xOffset);
			vertexBuffer.push_back(vertices[faceVertices * (int)face + faceLength * i + 1] + yOffset);
			vertexBuffer.push_back(vertices[faceVertices * (int)face + faceLength * i + 2] + zOffset);
			vertexBuffer.push_back(vertices[faceVertices * (int)face + faceLength * i + 3]);
			vertexBuffer.push_back(vertices[faceVertices * (int)face + faceLength * i + 4]);
		}
	}

	
private:
	static constexpr int faceLength = 5;
	static constexpr int faceVertices = 30;
	static const std::map<BlockType, BlockData> blockTypes;
	static constexpr float vertices[180] = {
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
};