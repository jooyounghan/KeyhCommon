#pragma once
// Internal D3D12 conversion helpers — not part of the public RHI interface.
// Include only from D3D12 implementation .cpp files.
#include "IRhiBuffer.h"

namespace keyh
{
	inline D3D12_RESOURCE_DESC toD3D12ResourceDesc(const RhiBufferDesc& desc)
	{
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
		InfoList infoList = D3D12ResourceFlagInfo::getInfoList(desc._resourceFlags);
		for (uint32 idx = 0; idx < infoList._count; ++idx)
		{
			const D3D12ResourceFlagInfo* resourceFlagInfo = infoList._items[idx];
			if (resourceFlagInfo != nullptr)
			{
				flags |= resourceFlagInfo->_flag;
			}
		}

		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12ResourceDimensionInfo::getInfo(desc._dimension)._dimension;
		resourceDesc.Width = desc._width;
		resourceDesc.Height = desc._height;
		resourceDesc.DepthOrArraySize = desc._depth;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = D3D12ResourceFormatInfo::getInfo(desc._format)._format;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Flags = flags;

		return resourceDesc;
	}

	inline D3D12_RESOURCE_STATES toD3D12ResourceStates(const RhiBufferDesc& desc)
	{
		D3D12_RESOURCE_STATES states = D3D12_RESOURCE_STATE_COMMON;
		InfoList infoList = D3D12ResourceStateInfo::getInfoList(desc._resourceStateFlags);
		for (uint32 idx = 0; idx < infoList._count; ++idx)
		{
			const D3D12ResourceStateInfo* resourceStateInfo = infoList._items[idx];
			if (resourceStateInfo != nullptr)
			{
				states |= resourceStateInfo->_state;
			}
		}
		return states;
	}
}
