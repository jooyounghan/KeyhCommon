#include "RenderRhiPch.h"
#include "IRhiDevice.h"
#include "IRhiSwapChain.h"
#include "IRhiCommandQueue.h"
#include "IRhiCommandPool.h"
#include "IRhiBuffer.h"

namespace keyh
{
	IDXGIFactory7* D3D12Device::getDxgiFactory() const
	{
		Microsoft::WRL::ComPtr<IDXGIFactory7> factory;
		_adapter->GetParent(IID_PPV_ARGS(&factory));
		return factory.Get();
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

		for (const D3D_FEATURE_LEVEL featureLevel : featureLevels)
		{
			hr = D3D12CreateDevice(_adapter.Get(), featureLevel, IID_PPV_ARGS(&_device));
			if (SUCCEEDED(hr))
			{
				return true;
			}
		}

		KEYH_ASSERT_ARGS(false, "Failed to create D3D12 device. HRESULT: 0x%X", hr);
		return false;
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
		return makePtr<D3D12SwapChain>(desc, getDxgiFactory(), presentQueue);
	}
	
	Ptr<IRhiBuffer> D3D12Device::createBuffer(const RhiBufferDesc& desc, EHeapType heapType)
	{
		return makePtr<D3D12Buffer>(_device.Get(), desc, heapType);
	}
	
	Ptr<IRhiTexture> D3D12Device::createTexture()
	{
		return {};
	}
	
	Ptr<IRhiSampler> D3D12Device::createSampler()
	{
		return {};
	}
	
	Ptr<IRhiFence> D3D12Device::createFence()
	{
		return {};
	}
	
	Ptr<IRhiGraphicsPipeline> D3D12Device::createGraphicsPipeline()
	{
		return {};
	}
	
	Ptr<IRhiComputePipeline> D3D12Device::createComputePipeline()
	{
		return {};
	}

	void D3D12Device::allocateMemoryHeap()
	{}
}
