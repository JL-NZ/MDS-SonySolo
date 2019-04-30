
#include <vector>
#include <gnmx.h>

#include "common/allocator.h"
#include "api_gnm/toolkit/toolkit.h"

#include "Terrain.h"

typedef struct Vertex
{
	float x, y, z; // Position
	float r, g, b;// Color
	float u, v;// UVs
	Vertex() {}
	Vertex(float _x, float _y, float _z, float _r, float _g, float _b, float _u, float _v) {
		x = _x; y = _y; z = _z; r = _r; g = _g; b = _b; u = _u; v = _v;
	}
} Vertex;

enum VertexElements
{
	kVertexPosition = 0,
	kVertexColor,
	kVertexUv,
	kVertexElemCount // element means attribute
};

#pragma once
class Utils
{
public:
	Utils();
	~Utils();

	static void setTriData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void setQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void setCubeData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void setSphereData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void setTerrainData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	
};

