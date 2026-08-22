#pragma once
// Internal D3D12 conversion helpers — not part of the public RHI interface.
// Include only from D3D12 implementation .cpp files.
#include "IRhiBuffer.h"
#include "IRhiTexture.h"
#include "IRhiSampler.h"

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

	inline D3D12_RESOURCE_STATES toD3D12ResourceStates(EResourceState resourceStateFlags)
	{
		D3D12_RESOURCE_STATES states = D3D12_RESOURCE_STATE_COMMON;
		InfoList infoList = D3D12ResourceStateInfo::getInfoList(resourceStateFlags);
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

	inline D3D12_RESOURCE_STATES toD3D12ResourceStates(const RhiBufferDesc& desc)
	{
		return toD3D12ResourceStates(desc._resourceStateFlags);
	}

	inline D3D12_RESOURCE_DESC toD3D12ResourceDesc(const RhiTextureDesc& desc)
	{
		KEYH_ASSERT(desc._depthOrArraySize <= UINT16_MAX, "Texture depth or array size exceeds D3D12 limits.");
		KEYH_ASSERT(desc._mipLevels <= UINT16_MAX, "Texture mip level count exceeds D3D12 limits.");

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
		resourceDesc.DepthOrArraySize = static_cast<uint16>(desc._depthOrArraySize);
		resourceDesc.MipLevels = static_cast<uint16>(desc._mipLevels);
		resourceDesc.Format = D3D12ResourceFormatInfo::getInfo(desc._format)._format;
		resourceDesc.SampleDesc.Count = desc._sampleCount == 0 ? 1 : desc._sampleCount;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resourceDesc.Flags = flags;

		return resourceDesc;
	}

	inline D3D12_RESOURCE_STATES toD3D12ResourceStates(const RhiTextureDesc& desc)
	{
		return toD3D12ResourceStates(desc._resourceStateFlags);
	}

	inline D3D12_SAMPLER_DESC toD3D12SamplerDesc(const RhiStaticSamplerDesc& desc)
	{
		D3D12_SAMPLER_DESC samplerDesc = {};
		
		samplerDesc.Filter = D3D12FilterInfo::getInfo(desc._filter)._filter;
		samplerDesc.AddressU = D3D12SamplerAddressModeInfo::getInfo(desc._addressU)._addressMode;
		samplerDesc.AddressV = D3D12SamplerAddressModeInfo::getInfo(desc._addressV)._addressMode;
		samplerDesc.AddressW = D3D12SamplerAddressModeInfo::getInfo(desc._addressW)._addressMode;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D12ComparisonFunctionInfo::getInfo(desc._comparisonFunc)._comparisonFunc;
		samplerDesc.BorderColor[0] = 0.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 0.0f;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		return samplerDesc;
	}
}
