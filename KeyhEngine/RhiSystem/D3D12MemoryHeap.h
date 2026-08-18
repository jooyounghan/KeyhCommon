#pragma once
#include "IRhiMemoryHeap.h"

namespace keyh
{
	class D3D12Device;

	class D3D12MemoryHeap : public IRhiMemoryHeap
	{
	public:
		D3D12MemoryHeap(D3D12Device* device, const RhiHeapDesc& desc);
		~D3D12MemoryHeap() override = default;

	public:
		inline ID3D12Heap* getNativeHeap() const { return _heap.Get(); }

	private:
		D3D12Device* _device;

	private:
		Microsoft::WRL::ComPtr<ID3D12Heap> _heap;

	public:
		virtual Ptr<IRhiBuffer> createBuffer(const RhiBufferDesc& desc) override;
		virtual Ptr<IRhiBuffer> createPlacedBuffer(const RhiBufferDesc& desc, uint32 offset) override;
	};
}
