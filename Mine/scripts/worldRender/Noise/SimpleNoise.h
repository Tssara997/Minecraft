#pragma once

#include <math.h>
#include <memory>

class SimplexNoise {
public:
	SimplexNoise(int seed) {
		createPerm(seed);
	}

	float simplexNoise3D(float x, float y, float z) {
		// przejscie do siatki simpleksowej

		float s = (x + y + z) * F3;
		vec3i skewed = { floor(x + s), floor(y + s), floor(z + s) };

		float t = (skewed.x + skewed.y + skewed.z) * G3;
		vec3f unskewed = { skewed.x - t, skewed.y - t, skewed.z - t };

		// wierzcholki simpleksu
		vec3f simplexCornersPos[4];
		vec3i simplexCornersIdx[4];

		simplexCornersPos[0] = { x - unskewed.x, y - unskewed.y, z - unskewed.z };
		vec3f V0 = simplexCornersPos[0];

		simplexCornersIdx[0] = { 0, 0, 0 };

		simplexCornersIdx[1] = { (V0.x >= V0.y && V0.y >= V0.z),
								(V0.x >= V0.y && V0.y >= V0.z),
								(V0.x >= V0.y && V0.y >= V0.z) };
		simplexCornersIdx[2] = { not(V0.x < V0.y && V0.y < V0.z),
								not(V0.x < V0.y && V0.y < V0.z),
								not(V0.x < V0.y && V0.y < V0.z) };

		simplexCornersIdx[3] = { 1, 1, 1 };

		simplexCornersPos[1] = { V0.x - simplexCornersIdx[1].x + G3, V0.y - simplexCornersIdx[1].y + G3, V0.z - simplexCornersIdx[1].z + G3 };
		simplexCornersPos[2] = { V0.x - simplexCornersIdx[2].x + 2.0f * G3, V0.y - simplexCornersIdx[2].y + 2.0f * G3, V0.z - simplexCornersIdx[2].z + 2.0f * G3 };
		simplexCornersPos[3] = { V0.x - 1 + 3 * G3, V0.y - 1 + 3 * G3, V0.z - 1 + 3 * G3 };

		//wybor gradientu
		float noise = 0.0f;
		for (uint16_t i{ 0 }; i < 4; i++) {
			uint16_t grad = perm[simplexCornersIdx[i].x & 255 + simplexCornersIdx[i].x + perm[simplexCornersIdx[i].y & 255 + simplexCornersIdx[i].y + perm[simplexCornersIdx[i].z & 255 + simplexCornersIdx[i].z]]];

			float t = 0.6 - simplexCornersPos[i].x * simplexCornersPos[i].x - simplexCornersPos[i].y * simplexCornersPos[i].y - simplexCornersPos[i].z * simplexCornersPos[i].z;
			if (t >= 0) {
				t *= t;
				noise += t * t * dot(GRADIENT3[grad], simplexCornersPos[i]);
			}
		}

		// normalizacja noise
		return 32 * noise;
	}

private:
	// Stale
	float F3 = 1 / 3;
	float G3 = 1 / 6;

	struct vec3f {
		float x, y, z;
	};

	struct vec3i {
		int x, y, z;
	};

	vec3i GRADIENT3[12] = {
		(1, 1, 0), (-1, 1, 0), (1,-1, 0), (-1,-1, 0),
		(1, 0, 1), (-1, 0, 1), (1, 0,-1), (-1, 0,-1),
		(0, 1, 1), (0,-1, 1), (0, 1,-1), (0,-1,-1)
	};

	float dot(vec3i i, vec3f j) {
		return i.x * j.x + i.y * j.y + i.z * j.z;
	}


	uint16_t perm[256];

	void createPerm(int seed) {
		srand(seed);
		uint16_t p[256];
		for (uint16_t i{ 0 }; i < 256; i++) {
			p[i] = i;
		}
		for (uint16_t i{ 0 }; i < 256; i++) {
			uint16_t swapIdx = rand() % 256;
			std::swap(p[i], p[swapIdx]);
			perm[i] = p[i];
		}
	}
};