#pragma once
#include "RhiEnum.h"

namespace keyh
{
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
		virtual void setViewports(uint32_t count, const RhiViewport* viewports) = 0;
		virtual void setScissorRects(uint32_t count, const RhiRect* rects) = 0;

	public:
		virtual void setRenderTargets(uint32_t rtvCount, const RhiCpuDescriptorHandle* rtvHandles, RhiCpuDescriptorHandle dsvHandle) = 0;
		virtual void clearRenderTargetView(RhiCpuDescriptorHandle rtvHandle, const float colorRgba[4]) = 0;
		virtual void clearDepthStencilView(RhiCpuDescriptorHandle dsvHandle, float depth, uint8_t stencil) = 0;

	public:
		virtual void drawInstanced(uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation) = 0;
		virtual void drawIndexedInstanced(uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndexLocation, int32_t baseVertexLocation, uint32_t startInstanceLocation) = 0;

	public:
		virtual void dispatch(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ) = 0;

	public:
		inline ECommandQueueType getQueueType() const { return _queueType; }

	protected:
		ECommandQueueType _queueType;
	};
}
