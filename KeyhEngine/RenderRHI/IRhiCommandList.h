#pragma once
namespace keyh
{
	class D3D12CommandPool;

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
		virtual void setViewports(uint32_t count, const D3D12_VIEWPORT* viewports) = 0;
		virtual void setScissorRects(uint32_t count, const D3D12_RECT* rects) = 0;

	public:
		virtual void setRenderTargets(uint32_t rtvCount, const D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) = 0;
		virtual void clearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float colorRgba[4]) = 0;
		virtual void clearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float depth, uint8_t stencil) = 0;

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
		virtual void setViewports(uint32_t count, const D3D12_VIEWPORT* viewports) override;
		virtual void setScissorRects(uint32_t count, const D3D12_RECT* rects) override;

	public:
		virtual void setRenderTargets(uint32_t rtvCount, const D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) override;
		virtual void clearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float colorRgba[4]) override;
		virtual void clearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float depth, uint8_t stencil) override;

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
