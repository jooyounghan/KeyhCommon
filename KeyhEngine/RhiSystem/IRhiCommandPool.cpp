#include "RhiSystemPch.h"
#include "D3D12CommandPool.h"
#include "D3D12Device.h"
#include "D3D12CommandList.h"

namespace keyh
{
	IRhiCommandPool::IRhiCommandPool(IRhiDevice* device, ECommandQueueType queueType)
		: _device(device)
		, _queueType(queueType)
	{}

	D3D12CommandPool::D3D12CommandPool(D3D12Device* device, ECommandQueueType queueType)
		: IRhiCommandPool(device, queueType)
		, _d3d12Device(device)
	{
		D3D12CommandQueueInfo commandQueueInfo = D3D12CommandQueueInfo::getInfo(queueType);
		_d3d12Device->getNativeDevice()->CreateCommandAllocator(
			commandQueueInfo._type, IID_PPV_ARGS(&_allocator)
		);
	}

	Ptr<IRhiCommandList> D3D12CommandPool::allocateCommandList()
	{
		return makePtr<D3D12CommandList>(this, _queueType);
	}

	void D3D12CommandPool::reset()
	{
		if (_allocator)
		{
			_allocator->Reset();
		}
	}
}
