#include "RenderRhiPch.h"
#include "IRhiBuffer.h"

namespace keyh
{
	D3D12_RESOURCE_DESC RhiBufferDesc::getD3D12ResourceDesc() const
	{
		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12ResourceDimensionInfo::getInfo(_dimension)._dimension;
		resourceDesc.Width = _width;
		resourceDesc.Height = _height;
		resourceDesc.DepthOrArraySize = _depth;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = D3D12ResourceFormatInfo::getInfo(_format)._format;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Flags = D3D12ResourceFlagInfo::getInfo(_flags)._flags;

		return resourceDesc;
	}

	IRhiBuffer::IRhiBuffer(const RhiBufferDesc& desc)
		: _desc(desc)
	{}

	D3D12Buffer::D3D12Buffer(ID3D12Device* device, const RhiBufferDesc& desc)
		: IRhiBuffer(desc)
	{}

	void* D3D12Buffer::map()
	{
		return nullptr;
	}
	void D3D12Buffer::unmap()
	{}
}
