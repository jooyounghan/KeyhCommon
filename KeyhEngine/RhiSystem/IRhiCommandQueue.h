#pragma once
#include "RhiEnum.h"

namespace keyh
{
	class IRhiDevice;
	class D3D12Device;

	class IRhiCommandQueue
	{
	public:
		IRhiCommandQueue(ECommandQueueType commandQueueType);
		virtual ~IRhiCommandQueue() = default;

	protected:
		ECommandQueueType _commandQueueType;

	public:
		virtual void executeCommandLists() = 0;
	};

	class D3D12CommandQueue : public IRhiCommandQueue
	{
	public:
		D3D12CommandQueue(D3D12Device* device, ECommandQueueType commandQueueType);
		~D3D12CommandQueue() override = default;

	public:
		virtual void executeCommandLists() override;

	private:
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> _commandQueue;
	};
}