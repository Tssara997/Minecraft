#include "chunk.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Noise/PerlinNoise.h"
#include "Noise/SimpleNoise.h"

Chunk::Chunk(short int xGlobalOffset, short int zGlobalOffset) : xGlobalOffset{ xGlobalOffset }, zGlobalOffset{ zGlobalOffset }, gpuUploaded{ false }, 
		highlands{ { 0.002f, 0.01f, 0.05f }, { 3.0f, 1.2f, 0.3f }, { 0.0f, 50.3f, 100.7f }, 2.5f, 1.0f, "highlands"},
		midlands{ { 0.02f, 0.01f, 0.2f }, { 0.8f, 0.1f, 0.1f }, { 0.0f, 50.3f, 100.7f }, 2.0f, 0.8f, "midlands" },
		lowlands{ { 0.003f, 0.05f, 0.001f}, { 0.5f, 0.1f, 0.2f }, { 0.0f, 50.3f, 100.7f }, 3.0f, 0.2f, "lowlands"}
{
	generateChunkData();
	generateMesh();
}

void Chunk::generateChunkData() {

	for (int x = 0; x < chunkSizeX; x++) {
		for (int z = 0; z < chunkSizeZ; z++) {
			double biomeNoise = perlinNoise::perlin2((x + xGlobalOffset * chunkSizeX) * 0.01f, (z + zGlobalOffset * chunkSizeZ) * 0.01f);
			double heightNoise{ 0 };
			if (lowlands.chance >= biomeNoise) {
				heightNoise = getNoiseheight(x, z, highlands);
				std::cout << "lowlands" << std::endl;
			}
			else if (biomeNoise < midlands.chance) {
				heightNoise = getNoiseheight(x, z, midlands);
				std::cout << "midlands" << std::endl;
			}
			else {
				heightNoise = getNoiseheight(x, z, highlands);
				std::cout << "highlands" << std::endl;
			}
			heightNoise = abs( 1.0f - heightNoise);
			heightNoise = floor(heightNoise * (topY - 1));
			for (int y = bottomY; y < topY; y++) {
				if (y < heightNoise)
					chunk[x][y][z] = Block::BlockType::DIRT;
				else if (y == heightNoise)
					chunk[x][y][z] = Block::BlockType::GRASSBLOCK;
				else
					chunk[x][y][z] = Block::BlockType::AIR;
			}
		}
	}
}

void Chunk::generateMesh() {
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

double Chunk::getNoiseheight(int x, int z, Biome_Data biom) const {
	double heightNoise{ 0 };
	for (int i{ 0 }; i < 3; i++)
		heightNoise += biom.amp[i] * perlinNoise::perlin2((x + xGlobalOffset * chunkSizeX)*biom.freq[i] + biom.offset[i], (z + zGlobalOffset * chunkSizeZ)*biom.freq[i] + biom.offset[i]);
	heightNoise /= (biom.amp[0] + biom.amp[1] + biom.amp[2]);
	//heightNoise = pow(heightNoise, biom.sinkess);
	heightNoise = 1.0f - fabs(heightNoise * 2.0 - 1.0);
	heightNoise *= heightNoise;
	return heightNoise;
}

void Chunk::draw(GLFWwindow* window) {
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

void Chunk::uploadToGPU() {
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	unsigned char* data = stbi_load("graphics\\textureAtlas.png", &width, &height, &channels, 0);
	if (!data) {
		std::cerr << "Failed to load dirt texture" << std::endl;
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	gpuUploaded = true;
}

void Chunk::clearMesh() {
	vertexBuffer.clear();
}

short int Chunk::getChunkPositonX() const {
	return xGlobalOffset;
}

short int Chunk::getChunkPositonZ() const {
	return zGlobalOffset;
}