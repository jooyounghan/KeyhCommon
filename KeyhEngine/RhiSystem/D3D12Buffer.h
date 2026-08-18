#pragma once
#include "IRhiBuffer.h"

namespace keyh
{
	class D3D12Device;

	class D3D12Buffer : public IRhiBuffer
	{
	public:
		D3D12Buffer(D3D12Device* device, const RhiBufferDesc& desc, EHeapType heapType = EHeapType::Default);
		D3D12Buffer(const RhiBufferDesc& desc, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
		~D3D12Buffer() override = default;

	public:
		virtual void* map() override;
		virtual void unmap() override;

	public:
		inline ID3D12Resource* getNativeResource() const
		{
			return _resource.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> _resource;
	};
}
