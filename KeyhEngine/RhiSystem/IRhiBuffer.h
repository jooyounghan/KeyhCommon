#pragma once
#include "RhiEnum.h"

namespace keyh
{
	struct RhiBufferDesc
	{
		uint32 _width = 0;
		uint32 _height = 0;
		uint32 _depth = 1;
		EResourceFormat _format = EResourceFormat::Unknown;
		EResourceDimension _dimension = EResourceDimension::Buffer;
		EResourceFlag _resourceFlags = EResourceFlag::None;
		EResourceState _resourceStateFlags = EResourceState::Common;

		uint32 getStride() const;
	};

	class IRhiBuffer
	{
	public:
		IRhiBuffer(const RhiBufferDesc& desc);
		virtual ~IRhiBuffer() = default;

	public:
		virtual void* map() = 0;
		virtual void unmap() = 0;

	public:
		inline const RhiBufferDesc& getDesc() const { return _desc; }

	protected:
		RhiBufferDesc _desc;
	};
}
