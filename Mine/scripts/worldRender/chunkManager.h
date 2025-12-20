#pragma once

#include "chunk.h"
#include <mutex>

class ChunkManager {
public:
	ChunkManager(short int renderDistance) : renderDistance(renderDistance) {
		generateChunks();
	}

	void generateChunks() { // thats only at the beggining and its done for now
		for (short int i = -renderDistance; i < renderDistance + 1; i++) {
			for (short int j = -renderDistance; j < renderDistance + 1; j++) {
				renderChunks.push_back(std::make_shared<Chunk>(i, j));
			}
		}

		{
			std::lock_guard<std::mutex> lock(chunksMutex);
			drawingChunks = renderChunks;
		}
	}

	void generateNewChunk() {
		short int xCenter = static_cast<short int>(std::floor(currentPos.x / 16.0f));
		short int zCenter = static_cast<short int>(std::floor(currentPos.z / 16.0f));
		
		for (short int x = xCenter - renderDistance; x < xCenter + renderDistance + 1; x++) {
			for (short int z = zCenter - renderDistance; z < zCenter + renderDistance + 1; z++) {
				if (!chunkExists(x, z))
					renderChunks.push_back(std::make_shared<Chunk>(x, z));
			}
		}
	}

	void updateChunks(const glm::vec3 pos) {
		if (generateNewChunks == false)
			return;
		glm::vec3 changePos = currentPos - pos;
		if (abs(changePos.x) <= 16.0f && abs(changePos.z) <= 16.0f)
			return;
		currentPos = pos;
		deleteFarChunks();
		generateNewChunk();
		std::cout << renderChunks.size() << std::endl;
		{
			std::lock_guard<std::mutex> lock(chunksMutex);
			drawingChunks = renderChunks;
		}
	}

	void deleteFarChunks() {
		// TODO this is temporary solution for deleting far chunks need to change
		for (auto& chunk : renderChunks) {
			if (!chunk)
				continue;
			short int chunkX = chunk->getChunkPositonX() * 16;
			short int chunkZ = chunk->getChunkPositonZ() * 16;
			if (abs(chunkX - currentPos.x) > renderDistance * 16 + 16 ||
				abs(chunkZ - currentPos.z) > renderDistance * 16 + 16) {
				chunk->clearMesh();
				chunk.reset();
			}

		}

		// temp
		for (uint16_t i{}; i < renderChunks.size(); i++) {
			if (!renderChunks.at(i)) {
				renderChunks.erase(renderChunks.begin() + i);
				i--;
				continue;
			}
		}
	}

	void draw(GLFWwindow* window) { // done
		std::vector<std::shared_ptr<Chunk>> local;
		{
			std::lock_guard<std::mutex> lock(chunksMutex);
			local = drawingChunks;
		}

		for (const auto& chunk : local) {
			if (!chunk) {
				continue;
			}
			chunk->draw(window);
		}
	}

	~ChunkManager() {
		std::lock_guard<std::mutex> lock(chunksMutex);
		drawingChunks.clear();
		renderChunks.clear();
	}

	void disableNewChunkGeneration() {
		generateNewChunks = false;
	}

private:

	bool chunkExists(short int xGlobal, short int zGlobal) const {
		for (const auto& chunk : renderChunks) {
			if (!chunk) continue;
			if (chunk->getChunkPositonX() == xGlobal && chunk->getChunkPositonZ() == zGlobal)
				return true;
		}
		return false;
	}	

	// TODO saving chunks, loading chunks
	short int renderDistance;
	std::vector<std::shared_ptr<Chunk>> drawingChunks;
	std::vector<std::shared_ptr<Chunk>> renderChunks;
	mutable std::mutex chunksMutex;
	glm::vec3 currentPos = { 0.0f, 52.0f, 0.0f };
	bool generateNewChunks = true;
};