#pragma once
#include "Ptr.h"
#include "RhiEnum.h"

namespace keyh
{
	class IRhiBuffer;
	class D3D12Device;
	struct RhiBufferDesc;

	struct RhiHeapDesc
	{
		uint32 _size = 0;
		EHeapType _heapType = EHeapType::Default;
		EHeapFlag _heapFlags = EHeapFlag::None;

#if defined(KEYH_PLATFORM_WINDOWS)
		D3D12_HEAP_DESC getD3D12HeapDesc() const;
		D3D12_HEAP_FLAGS getD3D12HeapFlags() const;
#endif
	};

	class IRhiMemoryHeap
	{
	public:
		IRhiMemoryHeap(const RhiHeapDesc& desc);
		virtual ~IRhiMemoryHeap() = default;

	public:
		inline uint32 getSize() const { return _desc._size; }
		inline uint32 getOffset() const { return _offset; }
		inline const RhiHeapDesc& getHeapDesc() const { return _desc; }

	protected:
		uint32 _offset = 0;
		RhiHeapDesc _desc;

	public:
		virtual Ptr<IRhiBuffer> createBuffer(const RhiBufferDesc& desc) = 0;
		virtual Ptr<IRhiBuffer> createPlacedBuffer(const RhiBufferDesc& desc, uint32 offset) = 0;
	};

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
