#include "RhiSystemPch.h"
#include "D3D12Device.h"
#include "D3D12CommandList.h"
#include "D3D12CommandPool.h"
#include "D3D12Sampler.h"
#include "D3D12GraphicsPipeline.h"
#include "D3D12ComputePipeline.h"
#include "D3D12PipelineLayout.h"
#include "D3D12RhiEnum.h"
#include "D3D12RhiConvert.h"

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

	void D3D12CommandList::setViewports(uint32 count, const RhiViewport* viewports)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "setViewports is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	InlinedVector<D3D12_VIEWPORT, 16> d3dViewports(count);
		//	for (uint32 i = 0; i < count; ++i)
		//	{
		//		d3dViewports[i] = { viewports[i].x, viewports[i].y, viewports[i].width, viewports[i].height, viewports[i].minDepth, viewports[i].maxDepth };
		//	}
		//	_commandList->RSSetViewports(count, d3dViewports.data());
		//}
	}

	void D3D12CommandList::setScissorRects(uint32 count, const RhiRect* rects)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "setScissorRects is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	InlinedVector<D3D12_RECT, 16> d3dRects(count);
		//	for (uint32 i = 0; i < count; ++i)
		//	{
		//		d3dRects[i] = { rects[i].left, rects[i].top, rects[i].right, rects[i].bottom };
		//	}
		//	_commandList->RSSetScissorRects(count, d3dRects.data());
		//}
	}

	void D3D12CommandList::setSampler(uint32 slot, IRhiSampler* sampler)
	{
		if (sampler == nullptr)
		{
			return;
		}

		D3D12Sampler* d3d12Sampler = static_cast<D3D12Sampler*>(sampler);
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = d3d12Sampler->getGpuHandle();

		_commandList->SetGraphicsRootDescriptorTable(slot, gpuHandle);
	}

	void D3D12CommandList::setRenderTargets(uint32 rtvCount, const RhiCpuDescriptorHandle* rtvHandles, RhiCpuDescriptorHandle dsvHandle)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "setRenderTargets is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	InlinedVector<D3D12_CPU_DESCRIPTOR_HANDLE, 8> d3dRtvHandles(rtvCount);
		//	for (uint32 i = 0; i < rtvCount; ++i)
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

	void D3D12CommandList::drawInstanced(uint32 vertexCountPerInstance, uint32 instanceCount, uint32 startVertexLocation, uint32 startInstanceLocation)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "drawInstanced is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	_commandList->DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
		//}
	}

	void D3D12CommandList::drawIndexedInstanced(uint32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32_t baseVertexLocation, uint32 startInstanceLocation)
	{
		//assert(_queueType == ECommandQueueType::Graphics && "drawIndexedInstanced is only valid on Graphics CommandList");
		//if (_commandList && _queueType == ECommandQueueType::Graphics)
		//{
		//	_commandList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
		//}
	}

	void D3D12CommandList::dispatch(uint32 threadGroupCountX, uint32 threadGroupCountY, uint32 threadGroupCountZ)
	{
		//assert((_queueType == ECommandQueueType::Graphics || _queueType == ECommandQueueType::Compute) && "dispatch is only valid on Graphics or Compute CommandList");
		//if (_commandList && (_queueType == ECommandQueueType::Graphics || _queueType == ECommandQueueType::Compute))
		//{
		//	_commandList->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
		//}
	}

	void D3D12CommandList::setPrimitiveTopology(EPrimitiveTopologyType topology)
	{
		_commandList->IASetPrimitiveTopology(D3D12PrimitiveTopologyTypeInfo::getInfo(topology)._topology);
	}

	void D3D12CommandList::setGraphicsPipeline(IRhiGraphicsPipeline* pipeline)
	{
		if (pipeline == nullptr)
		{
			return;
		}

		D3D12GraphicsPipeline* d3d12Pipeline = static_cast<D3D12GraphicsPipeline*>(pipeline);
		_commandList->SetPipelineState(d3d12Pipeline->getNativePipelineState());

		D3D12PipelineLayout* d3d12Layout = static_cast<D3D12PipelineLayout*>(pipeline->getPipelineLayout());
		if (d3d12Layout != nullptr)
		{
			_commandList->SetGraphicsRootSignature(d3d12Layout->getNativeRootSignature());
		}
	}

	void D3D12CommandList::setComputePipeline(IRhiComputePipeline* pipeline)
	{
		if (pipeline == nullptr)
		{
			return;
		}

		D3D12ComputePipeline* d3d12Pipeline = static_cast<D3D12ComputePipeline*>(pipeline);
		_commandList->SetPipelineState(d3d12Pipeline->getNativePipelineState());

		D3D12PipelineLayout* d3d12Layout = static_cast<D3D12PipelineLayout*>(pipeline->getPipelineLayout());
		if (d3d12Layout != nullptr)
		{
			_commandList->SetComputeRootSignature(d3d12Layout->getNativeRootSignature());
		}
	}

	void D3D12CommandList::setVertexBuffers(uint32 startSlot, uint32 count, const RhiVertexBufferView* views)
	{
		if (views == nullptr || count == 0)
		{
			return;
		}

		D3D12_VERTEX_BUFFER_VIEW d3dViews[D3D12_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
		const uint32 clampedCount = count < D3D12_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ? count : D3D12_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT;
		for (uint32 i = 0; i < clampedCount; ++i)
		{
			d3dViews[i] = toD3D12VertexBufferView(views[i]);
		}
		_commandList->IASetVertexBuffers(startSlot, clampedCount, d3dViews);
	}

	void D3D12CommandList::resourceBarrier(uint32 count, const RhiResourceBarrier* barriers)
	{
		if (barriers == nullptr || count == 0)
		{
			return;
		}

		D3D12_RESOURCE_BARRIER d3dBarriers[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT * 2];
		uint32 barrierCount = 0;

		for (uint32 i = 0; i < count && barrierCount < _countof(d3dBarriers); ++i)
		{
			d3dBarriers[barrierCount++] = toD3D12ResourceBarrier(barriers[i]);
		}

		if (barrierCount > 0)
		{
			_commandList->ResourceBarrier(barrierCount, d3dBarriers);
		}
	}
}