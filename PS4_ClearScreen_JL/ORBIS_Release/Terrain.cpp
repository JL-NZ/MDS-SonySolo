#include "Terrain.h"
#include "Utils.h"
#include "CCamera.h"
#include <random>

Terrain::Terrain()
{
	m_iNumCols = 513;
	m_iNumRows = 513;
	m_fHeightScale = 0.35f;
	m_fHeightOffset = -20.0f;
	m_fWidth = m_iNumCols;
	m_fDepth = m_iNumRows;
	m_strFilePath = "/app0/coastMountain513.raw";
}

Terrain::~Terrain()
{
}

void Terrain::Initialize()
{
	LoadHeightMap();
	Smooth();
	BuildVertexBuffer();
	BuildIndexBuffer();
}

void Terrain::BuildVertexBuffer()
{		
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(0.0f, 1.0f);

	float halfWidth = (m_iNumCols - 1) * 0.5f;
	float halfDepth = (m_iNumRows - 1) * 0.5f;	
	
	///std::vector<TerrainVertex> vertices((m_iNumCols * m_iNumRows));
	std::vector<Vertex> vertices((m_iNumCols * m_iNumRows));
	
	for (int i = 0; i < m_iNumRows; ++i)
	{
		float z = halfDepth - i;
	
		for (int j = 0; j < m_iNumCols; ++j)
		{
			float x = -halfWidth + j;
			float y = m_vecHeightMap[i * m_iNumCols + j];			
			float rgb = dis(gen);
			
			vertices[i * m_iNumCols + j].x = x;
			vertices[i * m_iNumCols + j].y = y;
			vertices[i * m_iNumCols + j].z = z;

			vertices[i * m_iNumCols + j].r = rgb; // generating a random number between 0.0f and 1.0f
			vertices[i * m_iNumCols + j].g = rgb;
			vertices[i * m_iNumCols + j].b = rgb;

			vertices[i * m_iNumCols + j].u = 0.0f;// x / m_fWidth;
			vertices[i * m_iNumCols + j].v = 0.0f;// y / m_fWidth;
		}
	}	
		
	m_vecVertices = vertices;
}

void Terrain::BuildIndexBuffer()
{		
	///std::vector<int> vecIndices(((m_iNumRows - 1) * (m_iNumCols - 1) * 2) * 3);
	std::vector<uint32_t> vecIndices(((m_iNumRows - 1) * (m_iNumCols - 1) * 2) * 3);

	int CurrentIndex = 0;
	
	for (int i = 0; i < m_iNumRows - 1; ++i)
	{
		for (int j = 0; j < m_iNumCols - 1; ++j)
		{
			vecIndices[CurrentIndex + 0] = i * m_iNumCols + j;
			vecIndices[CurrentIndex + 1] = i * m_iNumCols + j + 1;
			vecIndices[CurrentIndex + 2] = (i + 1) * m_iNumCols + j;
			
			vecIndices[CurrentIndex + 3] = (i + 1) * m_iNumCols + j;
			vecIndices[CurrentIndex + 4] = i * m_iNumCols + j + 1;
			vecIndices[CurrentIndex + 5] = (i + 1) * m_iNumCols + j + 1;
			
			CurrentIndex += 6; // next quad
		}
	}	

	m_vecIndices = vecIndices;
}

void Terrain::LoadHeightMap()
{
	// A height for each vertex
	std::vector<unsigned char> in(m_iNumRows * m_iNumCols);

	// Open the file.
	std::ifstream inFile;
	inFile.open(m_strFilePath.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array, and scale and offset the heights.
	m_vecHeightMap.resize(m_iNumRows * m_iNumCols, 0);
	for (int i = 0; i < m_iNumRows * m_iNumCols; ++i)
	{
		m_vecHeightMap[i] = static_cast<float>(in[i]) * m_fHeightScale + m_fHeightOffset;
	}
}

float Terrain::GetHeight(float x, float z) const
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*m_fWidth) / 1.0f;
	float d = (z - 0.5f*m_fDepth) / -1.0f;

	// Get the row and column we are in.
	int row = static_cast<int>(floorf(d));
	int col = static_cast<int>(floorf(c));
	
	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_vecHeightMap[row*m_iNumCols + col];
	float B = m_vecHeightMap[row*m_iNumCols + col + 1];
	float C = m_vecHeightMap[(row + 1)*m_iNumCols + col];
	float D = m_vecHeightMap[(row + 1)*m_iNumCols + col + 1];

	// Where we are relative to the cell.
	float s = c - static_cast<float>(col);
	float t = d - static_cast<float>(row);

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s * uy + t * vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s)*uy + (1.0f - t)*vy;
	}
}

void Terrain::Smooth()
{
	std::vector<float> vecSmoothedHeightmap(m_vecHeightMap.size());

	for (int i = 0; i < m_iNumRows; ++i)
	{		
		for (int j = 0; j < m_iNumCols; ++j)
		{
			vecSmoothedHeightmap[i * m_iNumCols + j] = Average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_vecHeightMap = vecSmoothedHeightmap;
}

bool Terrain::InBounds(int _a, int _b)
{
	return 
	(
		(_a >= 0 && _a < m_iNumRows) &&
		(_b >= 0 && _b < m_iNumCols)
	);
}

float Terrain::Average(int _a, int _b)
{
	// Function computes the average height of the AB element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |AB| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float fAvg = 0.0f;
	float fNum = 0.0f;

	for (int i = _a - 1; i <= _a + 1; ++i)
	{
		for (int j = _b - 1; j <= _b + 1; ++j)
		{
			if (InBounds(i, j))
			{
				fAvg += m_vecHeightMap[i * m_iNumCols + j];
				fNum += 1.0f;
			}
		}
	}

	return (fAvg * (1.0f / fNum));
}

