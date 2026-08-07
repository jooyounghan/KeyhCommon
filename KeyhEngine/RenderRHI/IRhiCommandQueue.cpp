#include "RenderRhiPch.h"
#include "IRhiCommandQueue.h"
#include "IRhiDevice.h"

namespace keyh
{
	IRhiCommandQueue::IRhiCommandQueue(ECommandQueueType commandQueueType)
		: _commandQueueType(commandQueueType)
	{
	
	}

	D3D12CommandQueue::D3D12CommandQueue(D3D12Device* device, ECommandQueueType commandQueueType)
		: IRhiCommandQueue(commandQueueType)
	{
		ID3D12Device* d3d12Device = device->getNativeDevice();
		
		D3D12CommandQueueInfo commandQueueInfo = D3D12CommandQueueInfo::getInfo(_commandQueueType);
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = commandQueueInfo._type;
		queueDesc.NodeMask = 0;
		queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		d3d12Device->CreateCommandQueue(
			&queueDesc,
			IID_PPV_ARGS(&_commandQueue)
		);
	}
	void D3D12CommandQueue::executeCommandLists()
	{}
}
