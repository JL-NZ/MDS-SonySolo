#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include <vector>

struct TerrainVertex
{
	glm::vec3 v3Pos;
	glm::vec3 v3Color;
};

class Terrain
{
public:
	Terrain();
	~Terrain();
	void Initialize();
	void BuildVertexBuffer();
	void BuildIndexBuffer();
	void LoadHeightMap();
	void LoadVegeMap();
	float GetHeight(float x, float z) const;
	void Smooth();
	bool InBounds(UINT _a, UINT _b);	
	float Average(UINT _a, UINT _b);
	void Render();
	void RenderGrass();

private:		
	GLuint m_vbo;
	GLuint m_vao;
	GLuint m_program;	
	GLuint m_grassProgram;
	GLuint m_texture;
	
	std::vector<TerrainVertex> m_vecVertices;
	std::vector<int> m_vecIndices;
	std::vector<float> m_vecHeightMap;
	std::vector<float> m_vecVegeMap;
	int m_iNumIndices;
	
	int m_iNumCols;
	int m_iNumRows;

	std::string m_strFilePath;
	float m_fHeightScale;
	float m_fHeightOffset;
	float m_fWidth;
	float m_fDepth;
};

