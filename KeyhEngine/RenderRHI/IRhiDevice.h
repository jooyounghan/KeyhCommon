#pragma once
namespace keyh
{
	class IRhiAdapter;
	class IRhiCommandQueue;
	class IRhiSwapChain;
	class IRhiBuffer;
	class IRhiTexture;
	class IRhiSampler;
	class IRhiFence;
	class IRhiGraphicsPipeline;
	class IRhiComputePipeline;

	struct RHISwapChainDesc;

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
		virtual IRhiCommandQueue*		createCommandQueue() = 0;
		virtual IRhiSwapChain*			createSwapChain(const RHISwapChainDesc& desc, IRhiCommandQueue* presentQueue) = 0;
		virtual IRhiBuffer*				createBuffer() = 0;
		virtual IRhiTexture*			createTexture() = 0;
		virtual IRhiSampler*			createSampler() = 0;
		virtual IRhiFence*				createFence() = 0;
		virtual IRhiGraphicsPipeline*	createGraphicsPipeline() = 0;
		virtual IRhiComputePipeline*	createComputePipeline() = 0;

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
		OwnerVector<IRhiCommandQueue>			_commandQueues;
		OwnerVector<IRhiSwapChain>				_swapChains;

	public:
		IDXGIFactory7*			getDxgiFactory() const;
		inline IDXGIAdapter4*	getDxgiAdapter() const { return _adapter.Get(); }
		inline ID3D12Device*	getD3D12Device() const { return _device.Get(); }

	public:
		virtual bool initialize() override;

	public:
		virtual IRhiCommandQueue*		createCommandQueue() override;
		virtual IRhiSwapChain*			createSwapChain(const RHISwapChainDesc& desc, IRhiCommandQueue* presentQueue) override;
		virtual IRhiBuffer*				createBuffer() override;
		virtual IRhiTexture*			createTexture() override;
		virtual IRhiSampler*			createSampler() override;
		virtual IRhiFence*				createFence() override;
		virtual IRhiGraphicsPipeline*	createGraphicsPipeline() override;
		virtual IRhiComputePipeline*	createComputePipeline() override;

	public:
		virtual void allocateMemoryHeap() override;


	};

}

