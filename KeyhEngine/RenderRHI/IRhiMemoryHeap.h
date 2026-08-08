#pragma once

namespace keyh
{
	class IRhiBuffer;
	class D3D12Device;
	struct RhiBufferDesc;

	class IRhiMemoryHeap
	{
	public:
		IRhiMemoryHeap(uint32 size, EHeapType heapType);
		virtual ~IRhiMemoryHeap() = default;

	public:
		inline uint32 getSize() const { return _size; }
		inline uint32 getOffset() const { return _offset; }
		inline EHeapType getHeapType() const { return _heapType; }

	protected:
		uint32 _size;
		uint32 _offset = 0;
		EHeapType _heapType;

	public:
		virtual void createBuffer(const RhiBufferDesc& desc, IRhiBuffer* buffer) = 0;
		virtual void createPlacedBuffer(const RhiBufferDesc& desc, uint32 offset, IRhiBuffer* buffer) = 0;
	};

	class D3D12MemoryHeap : public IRhiMemoryHeap
	{
	public:
		D3D12MemoryHeap(D3D12Device* device, uint32 size, EHeapType heapType);
		~D3D12MemoryHeap() override = default;

	public:
		inline ID3D12Heap* getNativeHeap() const { return _heap.Get(); }

	private:
		D3D12Device* _device;

	private:
		Microsoft::WRL::ComPtr<ID3D12Heap> _heap;

	public:
		virtual void createBuffer(const RhiBufferDesc& desc, IRhiBuffer* buffer) override;
		virtual void createPlacedBuffer(const RhiBufferDesc& desc, uint32 offset, IRhiBuffer* buffer) override;
	};
	
}

