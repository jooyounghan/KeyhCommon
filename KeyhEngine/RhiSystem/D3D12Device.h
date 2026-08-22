#pragma once
#include "IRhiDevice.h"

namespace keyh
{
	class D3D12GlobalResourceHeap;

	class D3D12Device : public IRhiDevice
	{
	public:
		D3D12Device(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter);
		virtual ~D3D12Device() = default;

	protected:
		Microsoft::WRL::ComPtr<IDXGIAdapter4>			_adapter;
		Microsoft::WRL::ComPtr<ID3D12Device>			_device;
		mutable Microsoft::WRL::ComPtr<IDXGIFactory7>	_factory;

	protected:
		Ptr<D3D12GlobalResourceHeap>		_cbvSrvUavHeap;
		Ptr<D3D12GlobalResourceHeap>		_samplerHeap;

	public:
		IDXGIFactory7*					getDxgiFactory() const;
		inline IDXGIAdapter4*			getDxgiAdapter() const { return _adapter.Get(); }
		inline ID3D12Device*			getNativeDevice() const { return _device.Get(); }
		inline D3D12GlobalResourceHeap* getCbvSrvUavHeap() const { return _cbvSrvUavHeap.get(); }
		inline D3D12GlobalResourceHeap* getSamplerHeap() const { return _samplerHeap.get(); }

	public:
		virtual bool initialize() override;

	public:
		virtual Ptr<IRhiCommandQueue>		createCommandQueue(ECommandQueueType commandQueueType) override;
		virtual Ptr<IRhiCommandPool>		createCommandPool(ECommandQueueType commandQueueType) override;
		virtual Ptr<IRhiSwapChain>			createSwapChain(const RHISwapChainDesc& desc, IRhiCommandQueue* presentQueue) override;
		virtual Ptr<IRhiBuffer>				createBuffer(const RhiBufferDesc& desc, EHeapType heapType) override;
		virtual Ptr<IRhiTexture>			createTexture(const RhiTextureDesc& desc, EHeapType heapType) override;
		virtual Ptr<IRhiSampler>			createSampler(const RhiStaticSamplerDesc& desc) override;
		virtual Ptr<IRhiFence>				createFence(const RhiFenceDesc& desc) override;
		virtual Ptr<IRhiGraphicsPipeline>	createGraphicsPipeline(const RhiGraphicsPipelineDesc& desc) override;
		virtual Ptr<IRhiComputePipeline>	createComputePipeline() override;

	public:
		virtual void allocateMemoryHeap() override;
	};
}
