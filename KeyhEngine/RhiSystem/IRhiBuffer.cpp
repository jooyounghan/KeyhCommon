#include "RhiSystemPch.h"
#include "D3D12Device.h"
#include "D3D12Buffer.h"
#include "D3D12RhiConvert.h"

namespace keyh
{
	uint32 RhiBufferDesc::getStride() const
	{
		ResourceFormatInfoBase resourceFormatInfo = D3D12ResourceFormatInfo::getInfo(_format);
		return resourceFormatInfo._bytesPerPixel;
	}

	IRhiBuffer::IRhiBuffer(const RhiBufferDesc& desc)
		: _desc(desc)
	{}

	D3D12Buffer::D3D12Buffer(D3D12Device* device, const RhiBufferDesc& desc, EHeapType heapType)
		: IRhiBuffer(desc)
	{
		D3D12_HEAP_PROPERTIES heapProperties = {};
		heapProperties.Type = D3D12HeapTypeInfo::getInfo(heapType)._heapType;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		const D3D12_RESOURCE_DESC resourceDesc = toD3D12ResourceDesc(desc);
		const HRESULT hr = device->getNativeDevice()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			toD3D12ResourceStates(desc),
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
