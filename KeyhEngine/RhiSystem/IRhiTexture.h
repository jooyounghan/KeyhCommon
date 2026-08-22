#pragma once
#include "RhiEnum.h"

namespace keyh
{
	struct RhiTextureDesc
	{
		uint32 _width = 1;
		uint32 _height = 1;
		uint32 _depthOrArraySize = 1;
		uint32 _mipLevels = 1;
		uint32 _sampleCount = 1;
		EResourceFormat _format = EResourceFormat::Unknown;
		EResourceDimension _dimension = EResourceDimension::Texture2D;
		EResourceFlag _resourceFlags = EResourceFlag::None;
		EResourceState _resourceStateFlags = EResourceState::Common;
	};

	class IRhiTexture
	{
	public:
		IRhiTexture(const RhiTextureDesc& desc);
		virtual ~IRhiTexture() = default;

	public:
		inline const RhiTextureDesc& getDesc() const { return _desc; }

	protected:
		RhiTextureDesc _desc;
	};
}