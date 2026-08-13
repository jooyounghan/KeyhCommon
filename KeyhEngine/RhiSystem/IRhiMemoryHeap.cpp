#include "RhiSystemPch.h"
#include "IRhiMemoryHeap.h"
#include "IRhiBuffer.h"
#include "D3D12RhiConvert.h"

namespace keyh
{
	static D3D12_HEAP_FLAGS toD3D12HeapFlags(const RhiHeapDesc& desc)
	{
		D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE;
		InfoList infoList = D3D12HeapFlagInfo::getInfoList(desc._heapFlags);
		for (uint32 idx = 0; idx < infoList._count; ++idx)
		{
			const D3D12HeapFlagInfo* heapFlagInfo = infoList._items[idx];
			if (heapFlagInfo != nullptr)
			{
				flags |= heapFlagInfo->_flag;
			}
		}
		return flags;
	}

	static D3D12_HEAP_DESC toD3D12HeapDesc(const RhiHeapDesc& desc)
	{
		D3D12_HEAP_DESC d3dDesc{};

		const D3D12HeapTypeInfo& typeInfo = D3D12HeapTypeInfo::getInfo(desc._heapType);

		d3dDesc.SizeInBytes = desc._size;
		d3dDesc.Alignment = 0;
		d3dDesc.Properties.Type = typeInfo._heapType;
		d3dDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		d3dDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		d3dDesc.Properties.CreationNodeMask = 1;
		d3dDesc.Properties.VisibleNodeMask = 1;
		d3dDesc.Flags = toD3D12HeapFlags(desc);
		return d3dDesc;
	}

	IRhiMemoryHeap::IRhiMemoryHeap(const RhiHeapDesc& desc)
		: _desc(desc)
	{}

	D3D12MemoryHeap::D3D12MemoryHeap(D3D12Device* device, const RhiHeapDesc& desc)
		: _device(device)
		, IRhiMemoryHeap(desc)
	{
		ID3D12Device* d3d12Device = _device->getNativeDevice();
        D3D12_HEAP_DESC heapDesc = toD3D12HeapDesc(desc); 
        d3d12Device->CreateHeap(&heapDesc, IID_PPV_ARGS(&_heap));
	}

	static bool isEnoughMemoryInHeap(const RhiHeapDesc& heapDesc, const RhiBufferDesc& bufferDesc, uint32 offset, uint32& requiredSize)
	{
		requiredSize = bufferDesc._width * bufferDesc._height * bufferDesc._depth * bufferDesc.getStride();
		if (offset + requiredSize > heapDesc._size)
		{
			KEYH_ASSERT_ARGS(false, "Not enough memory in the heap to create buffer.");
			return false;
		}
		return true;
	}

	Ptr<IRhiBuffer> D3D12MemoryHeap::createBuffer(const RhiBufferDesc& desc)
	{
		uint32 requiredSize = 0;
		if (isEnoughMemoryInHeap(_desc, desc, _offset, requiredSize))
		{
			Ptr<IRhiBuffer> buffer = createPlacedBuffer(desc, _offset);
			_offset += requiredSize;
			return buffer;
		}

		return nullptr;
	}

	Ptr<IRhiBuffer> D3D12MemoryHeap::createPlacedBuffer(const RhiBufferDesc& desc, uint32 offset)
	{
		uint32 requiredSize = 0;
		if (isEnoughMemoryInHeap(_desc, desc, offset, requiredSize))
		{
			D3D12_RESOURCE_DESC resourceDesc = toD3D12ResourceDesc(desc);
			Microsoft::WRL::ComPtr<ID3D12Resource> resource;
			const HRESULT hr = _device->getNativeDevice()->CreatePlacedResource(
				_heap.Get(),
				offset,
				&resourceDesc,
				toD3D12ResourceStates(desc),
				nullptr,
				IID_PPV_ARGS(&resource)
			);

			if (FAILED(hr))
			{
				KEYH_ASSERT_ARGS(false, "Failed to create placed buffer resource. HRESULT: 0x%X", hr);
				return nullptr;
			}
			return makePtr<IRhiBuffer, D3D12Buffer>(desc, keyh::move(resource));
		}
		return nullptr;
	}
}
