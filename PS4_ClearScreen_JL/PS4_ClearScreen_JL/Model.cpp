#include "Model.h"

#include "Utils.h"
#include "Render.h"

#include "common/shader_loader.h"
#include "GNFLoader.h"

bool readRawTextureData(const char *path, void *address, size_t size)
{
	bool success = false;

	FILE *fp = fopen(path, "rb");
	if (fp != NULL)
	{
		success = readFileContents(address, size, fp);
		fclose(fp);
	}

	return success;
}

Model::Model(ModelType modelType)
{
	translation = Vector3(0.0f, 0.0f, 0.0f);
	rotateAxis = Vector3(0.0f, 0.0f, 1.0f);
	scale = Vector3(0.0f, 0.0f, 0.0f);
	angle = 0.0f;
	
	switch (modelType)
	{
	case kTriangle:
		Utils::setTriData(vertices, indices);
		break;
	case kQuad:
		Utils::setQuadData(vertices, indices);
		break;
	case kCube:
		Utils::setCubeData(vertices, indices);
		break;
	case kSphere:
		Utils::setSphereData(vertices, indices);
		break;
	case kTerrain:
		Utils::setTerrainData(vertices, indices);
	default:
		break;
	}

	// Allocate the vertex buffer memory and assign vertex data
	Vertex* vertexData = static_cast<Vertex*>(Render::GetInstance()->garlicAllocator.allocate(sizeof(Vertex) * vertices.size(), Gnm::kAlignmentOfBufferInBytes));
	
	if (!vertexData)
	{
		printf("Cannot allocate vertex data\n");
	}

	// Copy the vertex/index data onto the GPU mapped memory
	memset(vertexData, 0, sizeof(Vertex) * vertices.size());
	memcpy(vertexData, &vertices[0], sizeof(Vertex) *	vertices.size());

	//set Position attribute
	vertexBuffers[kVertexPosition].initAsVertexBuffer(
		&vertexData->x, //base address 
		Gnm::kDataFormatR32G32B32Float, //format
		sizeof(Vertex), //stride
		sizeof(Vertex) * vertices.size() / sizeof(Vertex)
	); // no if elements


	//set Color attribute
	vertexBuffers[kVertexColor].initAsVertexBuffer(
		&vertexData->r, 
		Gnm::kDataFormatR32G32B32Float, 
		sizeof(Vertex),
		sizeof(Vertex) * vertices.size() / sizeof(Vertex)
	);

	//set UV attribute
	vertexBuffers[kVertexUv].initAsVertexBuffer(
		&vertexData->u, 
		Gnm::kDataFormatR32G32Float, 
		sizeof(Vertex), 
		sizeof(Vertex) * vertices.size() / sizeof(Vertex)
	);

	// Define index data
	indexData = static_cast<uint32_t*>(Render::GetInstance()->garlicAllocator.allocate(sizeof(uint32_t) * indices.size(),	Gnm::kAlignmentOfBufferInBytes));
	
	if (!indexData)
	{
		printf("Cannot allocate index data\n");
	}

	// Assign index data
	memset(indexData, 0, sizeof(uint32_t));
	memcpy(indexData, &indices[0], sizeof(uint32_t) * indices.size());	

	//Init raw textures
	//InitializeRawTextures();
	//InitializeGNFTextures("/app0/mytextures.gnf");
}

Model::~Model()
{
}

bool Model::genFetchShaderAndOffsetCache(std::string vertexBinary, std::string fragmentBinary)
{
	// Load shaders from file 
	vsShader = loadShaderFromFile<VsShader>(vertexBinary.c_str(), Render::GetInstance()->toolkitAllocators);
	psShader = loadShaderFromFile<PsShader>(fragmentBinary.c_str(), Render::GetInstance()->toolkitAllocators);
	if (!vsShader || !psShader)
	{
		printf("Cannot load the shaders\n");
	}

	// Allocate the memory for the fetch shader
	fsMem = Render::GetInstance()->garlicAllocator.allocate(Gnmx::computeVsFetchShaderSize(vsShader), Gnm::kAlignmentOfFetchShaderInBytes);
	if (!fsMem)
	{
		printf("Cannot allocate the fetch shader memory\n");
	}

	// Generate the fetch shader for the VS stage
	Gnmx::generateVsFetchShader(
		fsMem, // gets generated fetch shader
		&shaderModifier, // gets value to be passed to setVsShader
		vsShader,// pointer to vsShader binary
		NULL // default to vertex index
	);

	Gnmx::generateInputOffsetsCache(&vsInputOffsetsCache, Gnm::kShaderStageVs, vsShader);
	Gnmx::generateInputOffsetsCache(&psInputOffsetsCache, Gnm::kShaderStagePs, psShader);

	return true;
}

bool Model::InitializeRawTextures()
{
	Gnm::TextureSpec spec;
	spec.init();
	spec.m_textureType = Gnm::kTextureType2d;
	spec.m_width = 512;
	spec.m_height = 512;
	spec.m_depth = 1;
	spec.m_pitch = 0;
	spec.m_numMipLevels = 1;
	spec.m_numSlices = 1;
	spec.m_format = Gnm::kDataFormatR8G8B8A8UnormSrgb;
	spec.m_tileModeHint = Gnm::kTileModeDisplay_LinearAligned;
	spec.m_minGpuMode = Render::GetInstance()->gpuMode;
	spec.m_numFragments = Gnm::kNumFragments1;
	int32_t status = texture.init(&spec);

	if (status != SCE_GNM_OK)
	{
		printf("status != SCE_GNM_OK");
		return false;
	}
	textureSizeAlign = texture.getSizeAlign();

	// Allocate the texture data using the alignment returned by initAs2d
	void *textureData = Render::GetInstance()->garlicAllocator.allocate(textureSizeAlign);
	if (!textureData)
	{
		printf("Cannot allocate the texture data\n");
		return false;
	}

	// Read the texture data
	if (!readRawTextureData("/app0/texture.raw", textureData, textureSizeAlign.m_size))
	{
		printf("Cannot load the texture data\n");
		return false;
	}

	// Set the base data address in the texture object
	texture.setBaseAddress(textureData);

	sampler.init();
	sampler.setMipFilterMode(Gnm::kMipFilterModeNone);
	sampler.setXyFilterMode(Gnm::kFilterModeBilinear, Gnm::kFilterModeBilinear);
	return true;
}

bool Model::DrawRawTextures()
{
	// Get graphics context from render
	Gnmx::GnmxGfxContext &gfxc = Render::GetInstance()->renderContext->gfxContext;

	// Setup the texture and its sampler on the PS stage
	gfxc.setTextures(Gnm::kShaderStagePs, 0, 1, &texture);
	gfxc.setSamplers(Gnm::kShaderStagePs, 0, 1, &sampler);

	return true;
}

bool Model::InitializeGNFTextures(const char* _filePath)
{
	// Get graphics context from render
 
	GNFLoader::loadTextureFromGnf(
		&texture, 
		_filePath,
		0, 
		&Render::GetInstance()->toolkitAllocators
	);

	sampler.init(); 
	sampler.setMipFilterMode(Gnm::kMipFilterModeNone); 
	sampler.setXyFilterMode(Gnm::kFilterModeBilinear, Gnm::kFilterModeBilinear);

	return true;
}

bool Model::DrawGNFTextures()
{
	// Get graphics context from render
	Gnmx::GnmxGfxContext &gfxc = Render::GetInstance()->renderContext->gfxContext;

	gfxc.setTextures(
		Gnm::kShaderStagePs, // shader stage
		0, // slot to bind to 
		1, // no of slots to bind 
		&texture); // Gnm texture object to bind

	gfxc.setSamplers(
		Gnm::kShaderStagePs,
		0,
		1,
		&sampler);

	return false;
}


void Model::Draw()
{
	// Get graphics context from render
	Gnmx::GnmxGfxContext &gfxc = Render::GetInstance()->renderContext->gfxContext;

	// Activate VS and PS stages
	gfxc.setActiveShaderStages(Gnm::kActiveShaderStagesVsPs);
	gfxc.setVsShader(vsShader, shaderModifier, fsMem, &vsInputOffsetsCache);
	gfxc.setPsShader(psShader, &psInputOffsetsCache);

	// Set Vertex buffers / attributes
	gfxc.setVertexBuffers(Gnm::kShaderStageVs, 0, kVertexElemCount, vertexBuffers);	

	// Draw raw textures
	//DrawRawTextures();
	DrawGNFTextures();

	// Define constants
	ShaderConstants *constants = static_cast<ShaderConstants*>(gfxc.allocateFromCommandBuffer(sizeof(ShaderConstants), Gnm::kEmbeddedDataAlignment4));
	
	if (constants)
	{		
		// Angle increase to create constant rotation
		///angle+= 0.0002f;
		///if (angle > 360) angle = 0.0f;
		
		Matrix4 model = Matrix4::translation(translation) * Matrix4::rotation((angle * (180/M_PI)), rotateAxis) * Matrix4::scale(scale);
		Matrix4 projection = CCamera::GetInstance()->GetProjection();
		Matrix4 view = CCamera::GetInstance()->GetView();

		// Define WVP
		constants->m_WorldViewProj = ToMatrix4Unaligned(projection * view * model);

		// Init constant buffer
		Gnm::Buffer constBuffer;
		constBuffer.initAsConstantBuffer(constants, sizeof(ShaderConstants));

		// Set constant buffer to the VS stage
		gfxc.setConstantBuffers(
			Gnm::kShaderStageVs, // stage
			0, // start slot, first slot to bind to
			1, // num of slots to bind
			&constBuffer// buffer to bind
		); 
	}
	else
	{
		printf("Cannot allocate vertex shader constants\n");
	}

	// Set Primitive Type
	gfxc.setPrimitiveType(Gnm::kPrimitiveTypeTriList);

	// Set Index Siz
	gfxc.setIndexSize(Gnm::kIndexSize32);

	// Draw Index
	gfxc.drawIndex(indices.size(), indexData);	
}

