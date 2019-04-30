

#include <gnmx.h>
#include <video_out.h>

#include "api_gnm/toolkit/toolkit.h"
#include "common/allocator.h"

using namespace sce;
using namespace sce::Gnmx;

typedef struct RenderContext {
	Gnmx::GfxContext gfxContext;	// graphics context
	void *cueHeap;					// constant update engine
	void *dcbBuffer;				// draw command buffer
	void *ccbBuffer;				// constant command buffer
	volatile uint32_t *eopLabel;	// end of state label
	volatile uint32_t *contextLabel;// context label
} RenderContext;

struct DisplayBuffer
{
	Gnm::RenderTarget renderTarget;
	int displayIndex;
};

enum EopState // GPU finished working on the frame
{
	kEopStateNotYet = 0,
	kEopStateFinished,
};

enum RenderContextState
{
	kRenderContextFree = 0,
	kRenderContextInUse,
};

#pragma once
class Render
{
public:
	/// Variables

	// Constants
	const uint32_t kDisplayBufferWidth = 1920;
	const uint32_t kDisplayBufferHeight = 1080;
	static const uint32_t kDisplayBufferCount = 3;
	static const uint32_t kRenderContextCount = 2;
	const Gnm::ZFormat kZFormat = Gnm::kZFormat32Float;
	const Gnm::StencilFormat kStencilFormat = Gnm::kStencil8;
	const bool kHtileEnabled = true; // perform fast clears
	const Vector4 kClearColor = Vector4(0.0f, 1.0f, 0.7f, 1);
#if SCE_GNMX_ENABLE_GFX_LCUE
	static const uint32_t kNumLcueBatches = 100;
	static const size_t kDcbSizeInBytes = 2 * 1024 * 1024;
#else
	static const uint32_t kCueRingEntries = 64;
	static const size_t kDcbSizeInBytes = 2 * 1024 * 1024;
	static const size_t kCcbSizeInBytes = 256 * 1024;
#endif


	// GNM stuff
	Gnm::DepthRenderTarget depthTarget;
	Gnm::SizeAlign stencilSizeAlign;
	Gnm::SizeAlign htileSizeAlign;
	sce::Gnm::GpuMode gpuMode = Gnm::getGpuMode();
	sce::Gnmx::Toolkit::Allocators toolkitAllocators;

	LinearAllocator onionAllocator;
	LinearAllocator garlicAllocator;

	RenderContext *renderContext = renderContexts;
	RenderContext renderContexts[kRenderContextCount];
	uint32_t renderContextIndex = 0;
	
	SceKernelEqueue eopEventQueue;
	int videoOutHandle;	

	DisplayBuffer displayBuffers[kDisplayBufferCount];
	DisplayBuffer *backBuffer = displayBuffers;
	uint32_t backBufferIndex = 0;

	/// Member Functions
	void StartRender();
	void SetPipelineState();
	void EndRender();
	
// Singleton
public:
	static Render* GetInstance();
	void Destroy();
private:
	static Render* p_Render;

	Render();
	~Render();
};

