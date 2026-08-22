#pragma once
#include "IRhiTexture.h"

namespace keyh
{
	class D3D12Device;

	class D3D12Texture : public IRhiTexture
	{
	public:
		D3D12Texture(D3D12Device* device, const RhiTextureDesc& desc, EHeapType heapType = EHeapType::Default);
		D3D12Texture(const RhiTextureDesc& desc, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
		~D3D12Texture() override = default;

	public:
		inline ID3D12Resource* getNativeResource() const
		{
			return _resource.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> _resource;
	};
}
