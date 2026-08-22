#pragma once
#include "RhiEnum.h"

namespace keyh
{
	class IRhiSampler;

	class IRhiCommandList
	{
	public:
		IRhiCommandList(ECommandQueueType queueType)
			: _queueType(queueType)
		{}
		virtual ~IRhiCommandList() = default;

	public:
		virtual void begin() = 0;
		virtual void end() = 0;
		virtual void reset() = 0;

	public:
		virtual void setViewports(uint32 count, const RhiViewport* viewports) = 0;
		virtual void setScissorRects(uint32 count, const RhiRect* rects) = 0;
		virtual void setSampler(uint32 slot, IRhiSampler* sampler) = 0;

	public:
		virtual void setRenderTargets(uint32 rtvCount, const RhiCpuDescriptorHandle* rtvHandles, RhiCpuDescriptorHandle dsvHandle) = 0;
		virtual void clearRenderTargetView(RhiCpuDescriptorHandle rtvHandle, const float colorRgba[4]) = 0;
		virtual void clearDepthStencilView(RhiCpuDescriptorHandle dsvHandle, float depth, uint8_t stencil) = 0;

	public:
		virtual void drawInstanced(uint32 vertexCountPerInstance, uint32 instanceCount, uint32 startVertexLocation, uint32 startInstanceLocation) = 0;
		virtual void drawIndexedInstanced(uint32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32_t baseVertexLocation, uint32 startInstanceLocation) = 0;

	public:
		virtual void dispatch(uint32 threadGroupCountX, uint32 threadGroupCountY, uint32 threadGroupCountZ) = 0;

	public:
		inline ECommandQueueType getQueueType() const { return _queueType; }

	protected:
		ECommandQueueType _queueType;
	};
}
