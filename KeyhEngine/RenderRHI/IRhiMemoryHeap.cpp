#include "RenderRhiPch.h"
#include "IRhiMemoryHeap.h"
#include "IRhiBuffer.h"

namespace keyh
{
	IRhiMemoryHeap::IRhiMemoryHeap(uint32 size, EHeapType heapType)
		: _size(size)
		, _heapType(heapType)
	{}

	D3D12MemoryHeap::D3D12MemoryHeap(D3D12Device* device, uint32 size, EHeapType heapType)
		: _device(device)
		, IRhiMemoryHeap(size, heapType)
	{
		ID3D12Device* d3d12Device = _device->getNativeDevice();

        D3D12_HEAP_DESC heapDesc = {};
        heapDesc.SizeInBytes = size;
        heapDesc.Properties.Type = D3D12HeapTypeInfo::getInfo(heapType)._heapType;
        heapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
        heapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;

        d3d12Device->CreateHeap(&heapDesc, IID_PPV_ARGS(&_heap));
	}

	Ptr<IRhiBuffer> D3D12MemoryHeap::createBuffer(const RhiBufferDesc& desc)
	{
		return createPlacedBuffer(desc, 0);
	}

	Ptr<IRhiBuffer> D3D12MemoryHeap::createPlacedBuffer(const RhiBufferDesc& desc, uint32 offset)
	{
		D3D12_RESOURCE_DESC resourceDesc = desc.getD3D12ResourceDesc();
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		const HRESULT hr = _device->getNativeDevice()->CreatePlacedResource(
			_heap.Get(),
			offset,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&resource)
		);
		if (FAILED(hr))
		{
			KEYH_ASSERT_ARGS(false, "Failed to create placed buffer resource. HRESULT: 0x%X", hr);
			return {};
		}

		return makePtr<IRhiBuffer, D3D12Buffer>(desc, resource);
	}
}
