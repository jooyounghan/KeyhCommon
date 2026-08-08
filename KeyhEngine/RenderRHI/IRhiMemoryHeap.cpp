#include "RenderRhiPch.h"
#include "IRhiMemoryHeap.h"
#include "IRhiBuffer.h"

namespace keyh
{
#if defined(KEYH_PLATFORM_WINDOWS)
	D3D12_HEAP_DESC RhiHeapDesc::getD3D12HeapDesc() const
	{
		D3D12_HEAP_DESC desc{};

		const D3D12HeapTypeInfo& typeInfo = D3D12HeapTypeInfo::getInfo(_heapType);

		desc.SizeInBytes = _size;
		desc.Alignment = 0;
		desc.Properties.Type = typeInfo._heapType;
		desc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		desc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		desc.Properties.CreationNodeMask = 1;
		desc.Properties.VisibleNodeMask = 1;
		desc.Flags = getD3D12HeapFlags();
		return desc;
	}

	D3D12_HEAP_FLAGS RhiHeapDesc::getD3D12HeapFlags() const
	{
		D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE;
		InfoList infoList = D3D12HeapFlagInfo::getInfoList(_heapFlags);
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
#endif

	IRhiMemoryHeap::IRhiMemoryHeap(const RhiHeapDesc& desc)
		: _desc(desc)
	{}

	D3D12MemoryHeap::D3D12MemoryHeap(D3D12Device* device, const RhiHeapDesc& desc)
		: _device(device)
		, IRhiMemoryHeap(desc)
	{
		ID3D12Device* d3d12Device = _device->getNativeDevice();
        D3D12_HEAP_DESC heapDesc = _desc.getD3D12HeapDesc(); 
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
			D3D12_RESOURCE_DESC resourceDesc = desc.getD3D12ResourceDesc();
			Microsoft::WRL::ComPtr<ID3D12Resource> resource;
			const HRESULT hr = _device->getNativeDevice()->CreatePlacedResource(
				_heap.Get(),
				offset,
				&resourceDesc,
				desc.getD3D12ResourceStates(),
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
