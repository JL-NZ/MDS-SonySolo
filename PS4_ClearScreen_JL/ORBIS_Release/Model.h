#pragma once
#include <string>
#include <gnmx.h>

#include "Utils.h"
#include "std_cbuffer.h"
#include "CCamera.h"

using namespace sce;
using namespace sce::Gnmx;

enum ModelType
{
	kTriangle,
	kQuad,
	kCube,
	kSphere,
	kTerrain
};

//unistruct ShaderConstants
//{
//	Matrix4Unaligned m_WorldViewProj;
//};

#pragma once
class Model
{
public:
	/*Create model with default transforms.*/
	Model(ModelType modelType);		

	/*Create model with translate set by param, rest default.*/
	Model(ModelType modelType, const char* _filePath, Vector3 _translate) : Model(modelType)
	{ 
		translation = _translate;
		InitializeGNFTextures(_filePath);
	};

	/*Create model with scale and translation set by param, rest default.*/
	Model(ModelType modelType, const char* _filePath, Vector3 _translate, Vector3 _scale) : Model(modelType)
	{
		translation = _translate;
		scale = _scale;
		InitializeGNFTextures(_filePath);
	};

	/*Create model with scale, translate and rotation set by param.*/
	Model(ModelType modelType, const char* _filePath, Vector3 _translate, Vector3 _scale, Vector3 _rotateAxis, float _rotation) : Model(modelType)
	{
		translation = _translate;
		scale = _scale;
		rotateAxis = _rotateAxis;
		angle = _rotation;
		InitializeGNFTextures(_filePath);
	};

	~Model();

	// Member Functions
	void Draw();
	bool genFetchShaderAndOffsetCache(std::string vertexBinary, std::string fragmentBinary);
		
	Gnm::Buffer vertexBuffers[kVertexElemCount];

	VsShader *vsShader;
	PsShader *psShader;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	
	void *fsMem;
	uint32_t *indexData;
	uint32_t shaderModifier;

	sce::Gnmx::InputOffsetsCache vsInputOffsetsCache;
	sce::Gnmx::InputOffsetsCache psInputOffsetsCache;

	// Initialize a Gnm::Texture object
	Gnm::Texture texture;
	Gnm::SizeAlign textureSizeAlign;
	
	// Initialize the texture sampler
	Gnm::Sampler sampler;

	// Raw texture functions
	bool InitializeRawTextures();
	bool DrawRawTextures();

	// GNF texture functions
	bool InitializeGNFTextures(const char* _filePath);
	bool DrawGNFTextures();

	// Transform
	Vector3 translation;
	Vector3 rotateAxis;
	Vector3 scale;
	float angle;
};

