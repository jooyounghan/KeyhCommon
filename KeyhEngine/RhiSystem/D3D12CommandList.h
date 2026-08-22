#pragma once
#include "IRhiCommandList.h"

namespace keyh
{
	class D3D12CommandPool;

	class D3D12CommandList : public IRhiCommandList
	{
	public:
		D3D12CommandList(D3D12CommandPool* commandPool, ECommandQueueType queueType);
		~D3D12CommandList() override = default;

	public:
		virtual void begin() override;
		virtual void end() override;
		virtual void reset() override;

	public:
		virtual void setViewports(uint32 count, const RhiViewport* viewports) override;
		virtual void setScissorRects(uint32 count, const RhiRect* rects) override;
		virtual void setSampler(uint32 slot, IRhiSampler* sampler) override;

	public:
		virtual void setRenderTargets(uint32 rtvCount, const RhiCpuDescriptorHandle* rtvHandles, RhiCpuDescriptorHandle dsvHandle) override;
		virtual void clearRenderTargetView(RhiCpuDescriptorHandle rtvHandle, const float colorRgba[4]) override;
		virtual void clearDepthStencilView(RhiCpuDescriptorHandle dsvHandle, float depth, uint8_t stencil) override;

	public:
		virtual void drawInstanced(uint32 vertexCountPerInstance, uint32 instanceCount, uint32 startVertexLocation, uint32 startInstanceLocation) override;
		virtual void drawIndexedInstanced(uint32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32_t baseVertexLocation, uint32 startInstanceLocation) override;

	public:
		virtual void dispatch(uint32 threadGroupCountX, uint32 threadGroupCountY, uint32 threadGroupCountZ) override;

	public:
		inline ID3D12GraphicsCommandList* getNativeCommandList() const { return _commandList.Get(); }

	private:
		D3D12CommandPool*									_ownerPool;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	_commandList;
	};
}
