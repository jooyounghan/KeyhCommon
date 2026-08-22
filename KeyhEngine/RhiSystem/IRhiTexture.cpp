#include "RhiSystemPch.h"
#include "D3D12Device.h"
#include "D3D12Texture.h"
#include "D3D12RhiConvert.h"

namespace keyh
{
	IRhiTexture::IRhiTexture(const RhiTextureDesc& desc)
		: _desc(desc)
	{}

	D3D12Texture::D3D12Texture(const RhiTextureDesc& desc, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
		: IRhiTexture(desc)
		, _resource(keyh::move(resource))
	{}

	D3D12Texture::D3D12Texture(D3D12Device* device, const RhiTextureDesc& desc, EHeapType heapType)
		: IRhiTexture(desc)
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
		KEYH_ASSERT_ARGS(SUCCEEDED(hr), "Failed to create committed texture resource. HRESULT: 0x%X", hr);
	}
}
