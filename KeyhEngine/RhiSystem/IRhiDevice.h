#pragma once
#include "Ptr.h"
#include "RhiEnum.h"
namespace keyh
{
	class IRhiAdapter;
	class IRhiCommandQueue;
	class IRhiCommandPool;
	class IRhiSwapChain;
	class IRhiBuffer;
	class IRhiTexture;
	class IRhiSampler;
	class IRhiFence;
	class IRhiGraphicsPipeline;
	class IRhiComputePipeline;

	struct RHISwapChainDesc;
	struct RhiBufferDesc;
	struct RhiTextureDesc;
	struct RhiStaticSamplerDesc;
	struct RhiFenceDesc;
	struct RhiGraphicsPipelineDesc;
	struct RhiComputePipelineDesc;

	struct RhiAdapterInfo
	{
		StaticStringW	description;
		size_t			dedicatedVideoMemory = 0;
		size_t			dedicatedSystemMemory = 0;
		size_t			sharedSystemMemory = 0;
		uint32			vendorId = kInvalidUint32;
		uint32			deviceId = kInvalidUint32;
		bool			isSoftware = false;
	};

	class IRhiDevice
	{
	public:
		IRhiDevice() = default;
		virtual ~IRhiDevice() = default;

	protected:
		RhiAdapterInfo _adapterInfo;

	public:
		inline const RhiAdapterInfo& getInfo() const { return _adapterInfo; }

	public:
		virtual bool initialize() = 0;

	public:
		virtual Ptr<IRhiCommandQueue>	createCommandQueue(ECommandQueueType commandQueueType) = 0;
		virtual Ptr<IRhiCommandPool>	createCommandPool(ECommandQueueType commandQueueType) = 0;
		virtual Ptr<IRhiSwapChain>		createSwapChain(const RHISwapChainDesc& desc, IRhiCommandQueue* presentQueue) = 0;
		virtual Ptr<IRhiBuffer>			createBuffer(const RhiBufferDesc& desc, EHeapType heapType = EHeapType::Default) = 0;
		virtual Ptr<IRhiTexture>		createTexture(const RhiTextureDesc& desc, EHeapType heapType = EHeapType::Default) = 0;
		virtual Ptr<IRhiSampler>		createSampler(const RhiStaticSamplerDesc& desc) = 0;
		virtual Ptr<IRhiFence>			createFence(const RhiFenceDesc& desc) = 0;
		virtual Ptr<IRhiGraphicsPipeline> createGraphicsPipeline(const RhiGraphicsPipelineDesc& desc) = 0;
		virtual Ptr<IRhiComputePipeline> createComputePipeline(const RhiComputePipelineDesc& desc) = 0;

	public:
		virtual void					allocateMemoryHeap() = 0;
	};
}
