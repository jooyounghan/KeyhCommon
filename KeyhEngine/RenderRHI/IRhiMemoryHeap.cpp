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

	void D3D12MemoryHeap::createBuffer(const RhiBufferDesc& desc, IRhiBuffer* buffer)
	{
        D3D12_RESOURCE_DESC resourceDesc = desc.getD3D12ResourceDesc();
        //_device->getNativeDevice()->CreatePlacedResource(
        //    _heap.Get(),
        //    0,
        //    &resourceDesc,
        //    D3D12_RESOURCE_STATE_DEPTH_WRITE,
        //    nullptr,
        //    IID_PPV_ARGS(&_shadowMap)
        //);
	}

	void D3D12MemoryHeap::createPlacedBuffer(const RhiBufferDesc& desc, uint32 offset, IRhiBuffer* buffer)
	{
	
	}
}
