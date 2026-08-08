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
		virtual Ptr<IRhiTexture>		createTexture() = 0;
		virtual Ptr<IRhiSampler>		createSampler() = 0;
		virtual Ptr<IRhiFence>			createFence() = 0;
		virtual Ptr<IRhiGraphicsPipeline> createGraphicsPipeline() = 0;
		virtual Ptr<IRhiComputePipeline> createComputePipeline() = 0;

	public:
		virtual void					allocateMemoryHeap() = 0;
	};
	
	class D3D12Device : public IRhiDevice
	{
	public:
		D3D12Device(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter);
		virtual ~D3D12Device() = default;


	protected:
		Microsoft::WRL::ComPtr<IDXGIAdapter4>	_adapter;
		Microsoft::WRL::ComPtr<ID3D12Device>	_device;

	public:
		IDXGIFactory7*			getDxgiFactory() const;
		inline IDXGIAdapter4*	getDxgiAdapter() const { return _adapter.Get(); }
		inline ID3D12Device*	getNativeDevice() const { return _device.Get(); }

	public:
		virtual bool initialize() override;

	public:
		virtual Ptr<IRhiCommandQueue>	createCommandQueue(ECommandQueueType commandQueueType) override;
		virtual Ptr<IRhiCommandPool>	createCommandPool(ECommandQueueType commandQueueType) override;
		virtual Ptr<IRhiSwapChain>		createSwapChain(const RHISwapChainDesc& desc, IRhiCommandQueue* presentQueue) override;
		virtual Ptr<IRhiBuffer>			createBuffer(const RhiBufferDesc& desc, EHeapType heapType = EHeapType::Default) override;
		virtual Ptr<IRhiTexture>		createTexture() override;
		virtual Ptr<IRhiSampler>		createSampler() override;
		virtual Ptr<IRhiFence>			createFence() override;
		virtual Ptr<IRhiGraphicsPipeline> createGraphicsPipeline() override;
		virtual Ptr<IRhiComputePipeline> createComputePipeline() override;

	public:
		virtual void allocateMemoryHeap() override;


	};

}
