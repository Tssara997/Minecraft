#pragma once
#include <map>
#include <vector>
#include <array>

struct BlockData {
	int topTex[2];
	int sideTex[2];
	int botTex[2];
};

class Block {
public:
	enum class BlockType { AIR, DIRT, GRASSBLOCK, STONE, OAK_LOG, OAK_PLANKS };
	enum class Face { BACK, FRONT, LEFT, RIGHT, BOTTOM, UP };

	static void addVertices(std::vector<float>& vertexBuffer, BlockType block, Face face, int xOffset, int yOffset, int zOffset);
	static void createVerticesForBlock(BlockType block);

private:
	static constexpr float tileSize = 0.0625f;
	static constexpr int faceLength = 5;
	static constexpr int faceVertices = 30;
	static const std::map<BlockType, BlockData> blockTypes;
	static std::array<float, 180> vertices;
	static BlockType currentBlockVertices;
};