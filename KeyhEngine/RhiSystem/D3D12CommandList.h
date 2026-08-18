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
		virtual void setViewports(uint32_t count, const RhiViewport* viewports) override;
		virtual void setScissorRects(uint32_t count, const RhiRect* rects) override;

	public:
		virtual void setRenderTargets(uint32_t rtvCount, const RhiCpuDescriptorHandle* rtvHandles, RhiCpuDescriptorHandle dsvHandle) override;
		virtual void clearRenderTargetView(RhiCpuDescriptorHandle rtvHandle, const float colorRgba[4]) override;
		virtual void clearDepthStencilView(RhiCpuDescriptorHandle dsvHandle, float depth, uint8_t stencil) override;

	public:
		virtual void drawInstanced(uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation) override;
		virtual void drawIndexedInstanced(uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndexLocation, int32_t baseVertexLocation, uint32_t startInstanceLocation) override;

	public:
		virtual void dispatch(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ) override;

	public:
		inline ID3D12GraphicsCommandList* getNativeCommandList() const { return _commandList.Get(); }

	private:
		D3D12CommandPool*									_ownerPool;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	_commandList;
	};
}
