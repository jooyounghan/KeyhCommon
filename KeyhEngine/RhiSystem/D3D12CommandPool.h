#pragma once
#include "IRhiCommandPool.h"

namespace keyh
{
	class D3D12Device;

	class D3D12CommandPool : public IRhiCommandPool
	{
	public:
		D3D12CommandPool(D3D12Device* device, ECommandQueueType queueType);
		virtual ~D3D12CommandPool() override = default;

	public:
		virtual Ptr<IRhiCommandList> allocateCommandList() override;
		virtual void reset() override;

	public:
		inline D3D12Device*				getD3D12Device() const { return _d3d12Device; }
		inline ID3D12CommandAllocator*	getNativeCommandPool() const { return _allocator.Get(); }

	private:
		D3D12Device*									_d3d12Device = nullptr;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>	_allocator;
	};
}
