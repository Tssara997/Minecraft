#pragma once
#include <math.h>

namespace perlinNoise {

	struct vector3 {
		float x, y, z;
	};

	struct vector2 {
		float x, y;
	};

	vector3 normalize(vector3 v) {
		float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		v.x /= len;
		v.y /= len;
		v.z /= len;
		return v;
	}

	static const vector3 GRAD3[12] = {
	normalize({ 1, 1, 0}), normalize({-1, 1, 0}),
	normalize({ 1,-1, 0}), normalize({-1,-1, 0}),
	normalize({ 1, 0, 1}), normalize({-1, 0, 1}),
	normalize({ 1, 0,-1}), normalize({-1, 0,-1}),
	normalize({ 0, 1, 1}), normalize({ 0,-1, 1}),
	normalize({ 0, 1,-1}), normalize({ 0,-1,-1})
	};

	static const vector2 GRAD2[8] = {
		{1 / sqrt(2), 1 / sqrt(2)}, {-1 / sqrt(2), 1 / sqrt(2)}, {1 / sqrt(2), -1 / sqrt(2)}, {-1 / sqrt(2), -1 / sqrt(2)}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}
	};


	unsigned hash(int x, int y, int z) {
		unsigned h = x * 374761393u + y * 668265263u + z * 2147483647u;
		h = (h ^ (h >> 13u)) * 1274126177u;
		return h ^ (h >> 16u);
	}

	unsigned hash(int x, int y) {
		unsigned h = x * 374761393u + y * 668265263u;
		h = (h ^ (h >> 13)) * 1274126177u;
		return h ^ (h >> 16);
	}

	vector3 randomGradient(int ix, int iy, int iz) {
		unsigned h = hash(ix & 255, iy & 255, iz & 255);
		return GRAD3[h % 12];
	}

	vector2 randomGradient(int ix, int iy) {
		unsigned h = hash(ix, iy);
		return GRAD2[h % 8];
	}

	// computes the dot product of the distance and gradient vectors
	float dotGridGradient(int ix, int iy, int iz, float x, float y, float z) {
		// get gradient from intheger coordinates
		vector3 gradient = randomGradient(ix, iy, iz);

		// compute the distance vector
		float dx = x - static_cast<float>(ix);
		float dy = y - static_cast<float>(iy);
		float dz = z - static_cast<float>(iz);

		return dx * gradient.x + dy * gradient.y + dz * gradient.z;
	}

	float dotGridGradient(int ix, int iy, float x, float y) {
		vector2 gradient = randomGradient(ix, iy);

		float dx = x - static_cast<float>(ix);
		float dy = y - static_cast<float>(iy);

		return dx * gradient.x + dy * gradient.y;
	}

	// klasyczna funckja perlin noise czyli fade
	float fade(float t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	float interpolate(float a0, float a1, float w) {
		return a0 + (a1 - a0) * fade(w);
	}

	// Sample Perlin noises at coordinates x,y 
	float perlin3(float x, float y, float z) { // caves 
		// Determine grid cell corner coordinates
		//x = abs(x);
		//y = abs(y);
		//z = abs(z);
		int x0 = (int)floor(x);
		int y0 = (int)floor(y);
		int z0 = (int)floor(z);
		int x1 = x0 + 1;
		int y1 = y0 + 1;
		int z1 = z0 + 1;

		// interpelation weights
		float sx = x - static_cast<float>(x0);
		float sy = y - static_cast<float>(y0);
		float sz = z - static_cast<float>(z0);

		// Compute and interpolate top four corners
		float n0 = dotGridGradient(x0, y0, z0, x, y, z);
		float n1 = dotGridGradient(x1, y0, z0, x, y, z);
		float n2 = dotGridGradient(x0, y0, z1 ,x, y, z);
		float n3 = dotGridGradient(x1, y0, z1, x, y, z);
		float i0x0 = interpolate(n0, n1, sx);
		float i0x1 = interpolate(n2, n3, sx);

		// bottom two
		n0 = dotGridGradient(x0, y1, z0, x, y, z);
		n1 = dotGridGradient(x1, y1, z0, x, y, z);
		n2 = dotGridGradient(x0, y1, z1, x, y, z);
		n3 = dotGridGradient(x1, y1, z1, x, y, z);
		float i0x2 = interpolate(n0, n1, sx);
		float i0x3 = interpolate(n2, n3, sx);

		//interpolate between the two perviously interpolated values, now in y
		float i1x0 = interpolate(i0x0, i0x1, sy);
		float i1x1 = interpolate(i0x2, i0x3, sy);

		// final in the 3 dimention
		float value = interpolate(i1x0, i1x1, sz);

		return value;
	}


	float perlin2_raw(float x, float y) {
		// TODO this is a temp fix for negative values
		//x = abs(x);
		//y = abs(y);
		int x0 = (int)std::floor(x);
		int y0 = (int)std::floor(y);
		int x1 = x0 + 1;
		int y1 = y0 + 1;
		float sx = x - (float)(x0);
		float sy = y - (float)(y0);

		float n0 = dotGridGradient(x0, y0, x, y);
		float n1 = dotGridGradient(x1, y0, x, y);

		float ix0 = interpolate(n0, n1, sx);

		n0 = dotGridGradient(x0, y1, x, y);
		n1 = dotGridGradient(x1, y1, x, y);

		float ix1 = interpolate(n0, n1, sx);

		float value = interpolate(ix0, ix1, sy);

		return value;
	}


	float perlin2(float x, float y) {
		float value = perlin2_raw(x, y);
		
		// normalizacja do przedzialo [0, 1]
		value *= 0.9f;  
		value = value * 0.5f + 0.5f;

		if (value < 0.0f) value = 0.0f;
		if (value > 1.0f) value = 1.0f;
	
		return value;
	}

};