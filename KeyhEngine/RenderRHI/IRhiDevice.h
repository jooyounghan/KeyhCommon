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

	class IRhiDevice
	{
	public:
		IRhiDevice() = default;
		virtual ~IRhiDevice() = default;

	public:
		virtual bool initialize(IRhiAdapter* adapter) = 0;

	public:
		virtual IRhiCommandQueue*		createCommandQueue() = 0;
		virtual IRhiSwapChain*			createSwapChain() = 0;
		virtual IRhiBuffer*				createBuffer() = 0;
		virtual IRhiTexture*			createTexture() = 0;
		virtual IRhiSampler*			createSampler() = 0;
		virtual IRhiFence*				createFence() = 0;
		virtual IRhiGraphicsPipeline*	createGraphicsPipeline() = 0;
		virtual IRhiComputePipeline*	createComputePipeline() = 0;

	public:
		virtual void allocateMemoryHeap() = 0;
	};
	
	class D3D12Device : public IRhiDevice
	{
	public:
		D3D12Device() = default;
		virtual ~D3D12Device() = default;

	public:
		virtual bool initialize(IRhiAdapter* adapter) override;

	public:
		virtual IRhiCommandQueue* createCommandQueue() override;
		virtual IRhiSwapChain* createSwapChain() override;
		virtual IRhiBuffer* createBuffer() override;
		virtual IRhiTexture* createTexture() override;
		virtual IRhiSampler* createSampler() override;
		virtual IRhiFence* createFence() override;
		virtual IRhiGraphicsPipeline* createGraphicsPipeline() override;
		virtual IRhiComputePipeline* createComputePipeline() override;

	public:
		virtual void allocateMemoryHeap() override;

	protected:
		Microsoft::WRL::ComPtr<ID3D12Device>	_device;
	};

}

