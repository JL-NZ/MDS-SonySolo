#pragma once
#include <vector>
#include <fstream>
#include <gnmx.h>

class Terrain
{
public:
	Terrain();
	~Terrain();
	void Initialize();
	void BuildVertexBuffer();
	void BuildIndexBuffer();
	void LoadHeightMap();
	float GetHeight(float x, float z) const;
	void Smooth();
	bool InBounds(int _a, int _b);
	float Average(int _a, int _b);

	std::vector<struct Vertex> m_vecVertices;
	std::vector<uint32_t> m_vecIndices;

private:		
	///std::vector<TerrainVertex> m_vecVertices;
	///std::vector<int> m_vecIndices;

	std::vector<float> m_vecHeightMap;	
	
	int m_iNumCols;
	int m_iNumRows;

	std::string m_strFilePath;
	float m_fHeightScale;
	float m_fHeightOffset;
	float m_fWidth;
	float m_fDepth;
};

