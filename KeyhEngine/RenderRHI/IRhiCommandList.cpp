#include "RenderRhiPch.h"
#include "IRhiCommandList.h"
#include "IRhiCommandPool.h"

namespace keyh
{
	D3D12CommandList::D3D12CommandList(IRhiCommandPool* commandPool, ECommandQueueType queueType)
		: IRhiCommandList(queueType)
		, _ownerPool(commandPool)
	{
		D3D12CommandQueueInfo commandQueueInfo = D3D12CommandQueueInfo::getInfo(queueType);
		ID3D12Device* d3d12Device = static_cast<D3D12Device*>(commandPool->getDevice())->getNativeDevice();
		D3D12CommandPool* d3d12CommandPool = static_cast<D3D12CommandPool*>(commandPool);
		d3d12Device->CreateCommandList(0, commandQueueInfo._type, d3d12CommandPool->getNativeCommandPool(), nullptr, IID_PPV_ARGS(_commandList.GetAddressOf()));
	}

	void D3D12CommandList::begin()
	{}

	void D3D12CommandList::end()
	{
		if (_commandList)
		{
			_commandList->Close();
		}
	}

	void D3D12CommandList::reset()
	{
		//if (_commandList && _ownerPool)
		//{
		//	_commandList->Reset(_ownerPool->getNativeAllocator(), nullptr);
		//}
	}

	void D3D12CommandList::setViewports(uint32_t count, const D3D12_VIEWPORT* viewports)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "setViewports is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	_commandList->RSSetViewports(count, viewports);
		//}
	}

	void D3D12CommandList::setScissorRects(uint32_t count, const D3D12_RECT* rects)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "setScissorRects is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	_commandList->RSSetScissorRects(count, rects);
		//}
	}

	void D3D12CommandList::setRenderTargets(uint32_t rtvCount, const D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "setRenderTargets is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	_commandList->OMSetRenderTargets(rtvCount, rtvHandles, FALSE, &dsvHandle);
		//}
	}

	void D3D12CommandList::clearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float colorRgba[4])
	{
		//assert(_queueType == ECommandQueueType::Graphics && "clearRenderTargetView is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	_commandList->ClearRenderTargetView(rtvHandle, colorRgba, 0, nullptr);
		//}
	}

	void D3D12CommandList::clearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float depth, uint8_t stencil)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "clearDepthStencilView is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	_commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);
		//}
	}

	void D3D12CommandList::drawInstanced(uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "drawInstanced is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	_commandList->DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
		//}
	}

	void D3D12CommandList::drawIndexedInstanced(uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndexLocation, int32_t baseVertexLocation, uint32_t startInstanceLocation)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "drawIndexedInstanced is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	_commandList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
		//}
	}

	void D3D12CommandList::dispatch(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ)
	{
		//assert((_queueType == ECommandQueueType::Graphics || _queueType == ECommandQueueType::Compute) && "dispatch is only valid on Graphics or Compute CommandList");
		//if (_commandList && (_queueType == ECommandQueueType::Graphics || _queueType == ECommandQueueType::Compute))
		//{
		//	_commandList->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
		//}
	}
}