#pragma once
#include "IRhiCommandQueue.h"

namespace keyh
{
	class D3D12Device;

	class D3D12CommandQueue : public IRhiCommandQueue
	{
	public:
		D3D12CommandQueue(D3D12Device* device, ECommandQueueType commandQueueType);
		virtual ~D3D12CommandQueue() override = default;

	public:
		virtual void executeCommandLists() override;

	private:
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> _commandQueue;
	};
}
