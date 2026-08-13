#pragma once
#include "RhiEnum.h"

namespace keyh
{
	class IRhiCommandList;
	class IRhiDevice;
	class D3D12Device;

	class IRhiCommandPool
	{
	public:
		IRhiCommandPool(IRhiDevice* device, ECommandQueueType queueType);
		virtual ~IRhiCommandPool() = default;

	public:
		virtual IRhiCommandList* allocateCommandList() = 0;
		virtual void reset() = 0;

	public:
		inline IRhiDevice* getDevice() const { return _device; }
		inline ECommandQueueType getQueueType() const { return _queueType; }

	protected:
		IRhiDevice*			_device;
		ECommandQueueType	_queueType;
	};

	class D3D12CommandPool : public IRhiCommandPool
	{
	public:
		D3D12CommandPool(D3D12Device* device, ECommandQueueType queueType);
		~D3D12CommandPool() override = default;

	public:
		virtual IRhiCommandList* allocateCommandList() override;
		virtual void reset() override;

	public:
		inline D3D12Device*				getD3D12Device() const { return _d3d12Device; }
		inline ID3D12CommandAllocator*	getNativeCommandPool() const { return _allocator.Get(); }

	private:
		D3D12Device*									_d3d12Device = nullptr;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>	_allocator;
	};
}
