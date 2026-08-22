#include "RhiSystemPch.h"
#include "D3D12Device.h"
#include "D3D12SwapChain.h"
#include "D3D12CommandQueue.h"
#include "D3D12CommandPool.h"
#include "D3D12Buffer.h"
#include "D3D12Texture.h"
#include "D3D12Sampler.h"
#include "D3D12Fence.h"
#include "D3D12GraphicsPipeline.h"
#include "D3D12ComputePipeline.h"
#include "D3D12GlobalResourceHeap.h"

namespace keyh
{
	IDXGIFactory7* D3D12Device::getDxgiFactory() const
	{
		if (!_factory)
		{
			_adapter->GetParent(IID_PPV_ARGS(&_factory));
		}
		return _factory.Get();
	}

	bool D3D12Device::initialize()
	{
		DXGI_ADAPTER_DESC1 desc;
		HRESULT hr = _adapter->GetDesc1(&desc);
		if (FAILED(hr))
		{
			KEYH_ASSERT(false, "Failed to get adapter description.");
			return false;
		}

		_adapterInfo.description = desc.Description;
		_adapterInfo.dedicatedVideoMemory = desc.DedicatedVideoMemory;
		_adapterInfo.dedicatedSystemMemory = desc.DedicatedSystemMemory;
		_adapterInfo.sharedSystemMemory = desc.SharedSystemMemory;
		_adapterInfo.vendorId = desc.VendorId;
		_adapterInfo.deviceId = desc.DeviceId;
		_adapterInfo.isSoftware = (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0;

		constexpr D3D_FEATURE_LEVEL featureLevels[] = {
				D3D_FEATURE_LEVEL_12_2,
				D3D_FEATURE_LEVEL_12_1,
				D3D_FEATURE_LEVEL_12_0
		};

		bool isDeviceCreated = false;
		for (const D3D_FEATURE_LEVEL featureLevel : featureLevels)
		{
			hr = D3D12CreateDevice(_adapter.Get(), featureLevel, IID_PPV_ARGS(&_device));
			if (SUCCEEDED(hr))
			{
				isDeviceCreated = true;
				break;
			}
		}

		if (isDeviceCreated == false)
		{
			KEYH_ASSERT_ARGS(false, "Failed to create D3D12 device. HRESULT: 0x%X", hr);
			return false;
		}

		_cbvSrvUavHeap = makePtr<D3D12GlobalResourceHeap>(this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		_samplerHeap = makePtr<D3D12GlobalResourceHeap>(this, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 256);
		
		return true;
	}
	
	D3D12Device::D3D12Device(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter)
		: _adapter(adapter)
	{

	}

	Ptr<IRhiCommandQueue> D3D12Device::createCommandQueue(ECommandQueueType commandQueueType)
	{
		return makePtr<D3D12CommandQueue>(this, commandQueueType);
	}

	Ptr<IRhiCommandPool> D3D12Device::createCommandPool(ECommandQueueType commandQueueType)
	{
		return makePtr<D3D12CommandPool>(this, commandQueueType);
	}
	
	Ptr<IRhiSwapChain> D3D12Device::createSwapChain(const RHISwapChainDesc& desc, IRhiCommandQueue* presentQueue)
	{
		return makePtr<D3D12SwapChain>(desc, this, presentQueue);
	}
	
	Ptr<IRhiBuffer> D3D12Device::createBuffer(const RhiBufferDesc& desc, EHeapType heapType)
	{
		return makePtr<D3D12Buffer>(this, desc, heapType);
	}
	
	Ptr<IRhiTexture> D3D12Device::createTexture(const RhiTextureDesc& desc, EHeapType heapType)
	{
		return makePtr<D3D12Texture>(this, desc, heapType);
	}
	
	Ptr<IRhiSampler> D3D12Device::createSampler(const RhiStaticSamplerDesc& desc)
	{
		return makePtr<D3D12Sampler>(this, desc);
	}
	
	Ptr<IRhiFence> D3D12Device::createFence(const RhiFenceDesc& desc)
	{
		return makePtr<D3D12Fence>(this, desc);
	}
	
	Ptr<IRhiGraphicsPipeline> D3D12Device::createGraphicsPipeline(const RhiGraphicsPipelineDesc& desc)
	{
		return makePtr<D3D12GraphicsPipeline>(this, desc);
	}
	
	Ptr<IRhiComputePipeline> D3D12Device::createComputePipeline(const RhiComputePipelineDesc& desc)
	{
		return makePtr<D3D12ComputePipeline>(this, desc);
	}

	void D3D12Device::allocateMemoryHeap()
	{}
}
