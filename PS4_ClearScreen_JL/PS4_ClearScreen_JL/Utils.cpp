#include "Utils.h"



Utils::Utils()
{
}


Utils::~Utils()
{
}

void Utils::setTriData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	std::vector<Vertex> Vertices;	
	Vertices.push_back(Vertex(-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f));
	Vertices.push_back(Vertex(0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f));
	Vertices.push_back(Vertex(-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f));
	
	std::vector<uint32_t> Indices;
	Indices.push_back(0);
	Indices.push_back(2);
	Indices.push_back(1);

	vertices.clear(); indices.clear();

	vertices = Vertices;
	indices = Indices;
}

void Utils::setQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	std::vector<Vertex> Vertices;
	Vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	Vertices.push_back(Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
	Vertices.push_back(Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
	Vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));

	std::vector<uint32_t> Indices
	{
		0, 1, 2,
		0, 2, 3,
	};

	vertices.clear(); indices.clear();

	vertices = Vertices;
	indices = Indices;
}

void Utils::setCubeData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	std::vector<Vertex> Vertices;

	// Front Face
	Vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f));
	Vertices.push_back(Vertex(0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f));
	Vertices.push_back(Vertex(0.5f, -0.5f, 0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f));
	Vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f));

	// Right Face
	Vertices.push_back(Vertex(0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f));
	Vertices.push_back(Vertex(0.5f, 0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f));
	Vertices.push_back(Vertex(0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f));
	Vertices.push_back(Vertex(0.5f, -0.5f, 0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f));

	// Back Face
	Vertices.push_back(Vertex(0.5f, 0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f));
	Vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f));
	Vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f));
	Vertices.push_back(Vertex(0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f));

	// Left Face
	Vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 0.0f));
	Vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f));
	Vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f));
	Vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f));

	// Top Face
	Vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f,	0.0f, 1.0f, 1.0f,	0.0f, 0.0f));
	Vertices.push_back(Vertex(0.5f, 0.5f, -0.5f,	0.0f, 1.0f, 1.0f,	1.0f, 0.0f));
	Vertices.push_back(Vertex(0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 1.0f,	1.0f, 1.0f));
	Vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f));

	// Bottom Face
	Vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f));
	Vertices.push_back(Vertex(0.5f, -0.5f, 0.5f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f));
	Vertices.push_back(Vertex(0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f));
	Vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f));

	std::vector<uint32_t> Indices
	{
		// Front Face
		0, 1, 2,
		0, 2, 3,

		// Right Face
		4, 5, 6,
		4, 6, 7,

		// Back Face
		8, 9, 10,
		8, 10, 11,

		// Left Face
		12, 13, 14,
		12, 14, 15,

		// Top Face
		16, 17, 18,
		16, 18, 19,

		// Bottom Face
		20, 21, 22,
		20, 22, 23
	};

	vertices.clear();
	indices.clear();

	vertices = Vertices;
	indices = Indices;
}

void Utils::setSphereData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;

	float radius = 1.5f;

	const int sections = 21;

	double phi = 0;
	double theta = 0;

	int offset = 0;
	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = static_cast<float>(cos(phi) * sin(theta));
			float y = static_cast<float>(cos(theta));
			float z = static_cast<float>(sin(phi) * sin(theta));

			Vertex vert;
			vert.x = x * radius;
			vert.y = y * radius;
			vert.z = z * radius;

			vert.r = x;
			vert.g = y;
			vert.b = z;

			vert.u = (float)i / (sections - 1);
			vert.v = (float)j / (sections - 1);

			Vertices.push_back(vert);

			theta += (M_PI / (sections - 1));
		}

		phi += (2 * M_PI) / (sections - 1);
	}


	offset = 0;
	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			Indices.push_back((((i + 1) % sections) * sections) + ((j + 1) % sections));
			Indices.push_back((((i + 1) % sections) * sections) + (j));
			Indices.push_back((i * sections) + (j));

			Indices.push_back((i * sections) + ((j + 1) % sections));
			Indices.push_back((((i + 1) % sections) * sections) + ((j + 1) % sections));
			Indices.push_back((i * sections) + (j));
		}
	}

	vertices.clear();
	indices.clear();

	vertices = Vertices;
	indices = Indices;
}

void Utils::setTerrainData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	Terrain* pTerrain = new Terrain();
	pTerrain->Initialize();

	std::vector<Vertex> Vertices;
	for (int i = 0; i < pTerrain->m_vecVertices.size(); i++)
	{
		Vertices.push_back(Vertex(pTerrain->m_vecVertices[i]));
	}


	std::vector<uint32_t> Indices = pTerrain->m_vecIndices;

	delete pTerrain;
	pTerrain = nullptr;

	vertices.clear(); indices.clear();

	vertices = Vertices;
	indices = Indices;
}

