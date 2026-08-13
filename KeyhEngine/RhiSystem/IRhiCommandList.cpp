#include "RhiSystemPch.h"
#include "IRhiCommandList.h"
#include "IRhiCommandPool.h"

namespace keyh
{
	D3D12CommandList::D3D12CommandList(D3D12CommandPool* commandPool, ECommandQueueType queueType)
		: IRhiCommandList(queueType)
		, _ownerPool(commandPool)
	{
		D3D12CommandQueueInfo commandQueueInfo = D3D12CommandQueueInfo::getInfo(queueType);
		ID3D12Device* d3d12Device = commandPool->getD3D12Device()->getNativeDevice();
		d3d12Device->CreateCommandList(0, commandQueueInfo._type, commandPool->getNativeCommandPool(), nullptr, IID_PPV_ARGS(_commandList.GetAddressOf()));
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

	void D3D12CommandList::setViewports(uint32_t count, const RhiViewport* viewports)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "setViewports is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	InlinedVector<D3D12_VIEWPORT, 16> d3dViewports(count);
		//	for (uint32_t i = 0; i < count; ++i)
		//	{
		//		d3dViewports[i] = { viewports[i].x, viewports[i].y, viewports[i].width, viewports[i].height, viewports[i].minDepth, viewports[i].maxDepth };
		//	}
		//	_commandList->RSSetViewports(count, d3dViewports.data());
		//}
	}

	void D3D12CommandList::setScissorRects(uint32_t count, const RhiRect* rects)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "setScissorRects is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	InlinedVector<D3D12_RECT, 16> d3dRects(count);
		//	for (uint32_t i = 0; i < count; ++i)
		//	{
		//		d3dRects[i] = { rects[i].left, rects[i].top, rects[i].right, rects[i].bottom };
		//	}
		//	_commandList->RSSetScissorRects(count, d3dRects.data());
		//}
	}

	void D3D12CommandList::setRenderTargets(uint32_t rtvCount, const RhiCpuDescriptorHandle* rtvHandles, RhiCpuDescriptorHandle dsvHandle)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "setRenderTargets is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	InlinedVector<D3D12_CPU_DESCRIPTOR_HANDLE, 8> d3dRtvHandles(rtvCount);
		//	for (uint32_t i = 0; i < rtvCount; ++i)
		//	{
		//		d3dRtvHandles[i] = { rtvHandles[i].ptr };
		//	}
		//	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvHandle{ dsvHandle.ptr };
		//	_commandList->OMSetRenderTargets(rtvCount, d3dRtvHandles.data(), FALSE, &d3dDsvHandle);
		//}
	}

	void D3D12CommandList::clearRenderTargetView(RhiCpuDescriptorHandle rtvHandle, const float colorRgba[4])
	{
		//assert(_queueType == ECommandQueueType::Graphics && "clearRenderTargetView is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	D3D12_CPU_DESCRIPTOR_HANDLE d3dHandle{ rtvHandle.ptr };
		//	_commandList->ClearRenderTargetView(d3dHandle, colorRgba, 0, nullptr);
		//}
	}

	void D3D12CommandList::clearDepthStencilView(RhiCpuDescriptorHandle dsvHandle, float depth, uint8_t stencil)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "clearDepthStencilView is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	D3D12_CPU_DESCRIPTOR_HANDLE d3dHandle{ dsvHandle.ptr };
		//	_commandList->ClearDepthStencilView(d3dHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);
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