#include "RenderRhiPch.h"
#include "IRhiBuffer.h"

namespace keyh
{
	D3D12_RESOURCE_DESC RhiBufferDesc::getD3D12ResourceDesc() const
	{
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
		InfoList infoList = D3D12ResourceFlagInfo::getInfoList(_flags);
		for (uint32 idx = 0; idx < infoList._count; ++idx)
		{
			const D3D12ResourceFlagInfo* resourceFlagInfo = infoList._items[idx];
			if (resourceFlagInfo != nullptr)
			{
				flags |= resourceFlagInfo->_flag;
			}
		}

		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12ResourceDimensionInfo::getInfo(_dimension)._dimension;
		resourceDesc.Width = _width;
		resourceDesc.Height = _height;
		resourceDesc.DepthOrArraySize = _depth;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = D3D12ResourceFormatInfo::getInfo(_format)._format;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Flags = flags;

		return resourceDesc;
	}

	IRhiBuffer::IRhiBuffer(const RhiBufferDesc& desc)
		: _desc(desc)
	{}

	D3D12Buffer::D3D12Buffer(ID3D12Device* device, const RhiBufferDesc& desc, EHeapType heapType)
		: IRhiBuffer(desc)
	{
		D3D12_HEAP_PROPERTIES heapProperties = {};
		heapProperties.Type = D3D12HeapTypeInfo::getInfo(heapType)._heapType;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		const D3D12_RESOURCE_DESC resourceDesc = desc.getD3D12ResourceDesc();
		const HRESULT hr = device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&_resource)
		);
		KEYH_ASSERT_ARGS(SUCCEEDED(hr), "Failed to create committed buffer resource. HRESULT: 0x%X", hr);
	}

	D3D12Buffer::D3D12Buffer(const RhiBufferDesc& desc, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
		: IRhiBuffer(desc)
		, _resource(keyh::move(resource))
	{}

	void* D3D12Buffer::map()
	{
		if (_resource == nullptr)
		{
			return nullptr;
		}

		void* mappedData = nullptr;
		const HRESULT hr = _resource->Map(0, nullptr, &mappedData);
		if (FAILED(hr))
		{
			KEYH_ASSERT_ARGS(false, "Failed to map buffer resource. HRESULT: 0x%X", hr);
			return nullptr;
		}
		return mappedData;
	}

	void D3D12Buffer::unmap()
	{
		if (_resource != nullptr)
		{
			_resource->Unmap(0, nullptr);
		}
	}
}
