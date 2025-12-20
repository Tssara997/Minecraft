#include "block.h"

const std::map<Block::BlockType, BlockData> Block::blockTypes = {
		{ BlockType::AIR, { -1, -1, -1 } },
		{ BlockType::DIRT, { 2, 2, 2 } },
		{ BlockType::GRASSBLOCK, { 0, 2, 3 } },
		{ BlockType::STONE, { 1, 1, 1 } },
		{ BlockType::OAK_LOG, { 21, 20, 21 } },
		{ BlockType::OAK_PLANKS, { 4, 4, 4 } }
};