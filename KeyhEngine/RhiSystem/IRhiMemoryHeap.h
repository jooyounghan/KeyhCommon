#pragma once
#include "Ptr.h"
#include "RhiEnum.h"

namespace keyh
{
	class IRhiBuffer;
	struct RhiBufferDesc;

	struct RhiHeapDesc
	{
		uint32 _size = 0;
		EHeapType _heapType = EHeapType::Default;
		EHeapFlag _heapFlags = EHeapFlag::None;
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
}
