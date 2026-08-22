#pragma once
// Internal D3D12 conversion helpers — not part of the public RHI interface.
// Include only from D3D12 implementation .cpp files.
#include "IRhiBuffer.h"
#include "IRhiTexture.h"
#include "IRhiSampler.h"
#include "IRhiGraphicsPipeline.h"

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

	inline D3D12_RASTERIZER_DESC toD3D12RasterizerDesc(const RhiRasterizerDesc& desc)
	{
		D3D12_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode              = D3D12FillModeInfo::getInfo(desc._fillMode)._fillMode;
		rasterizerDesc.CullMode              = D3D12CullModeInfo::getInfo(desc._cullMode)._cullMode;
		rasterizerDesc.FrontCounterClockwise = desc._frontCounterClockwise ? TRUE : FALSE;
		rasterizerDesc.DepthBias             = desc._depthBias;
		rasterizerDesc.DepthBiasClamp        = desc._depthBiasClamp;
		rasterizerDesc.SlopeScaledDepthBias  = desc._slopeScaledDepthBias;
		rasterizerDesc.DepthClipEnable       = desc._depthClipEnable ? TRUE : FALSE;
		rasterizerDesc.MultisampleEnable     = desc._multisampleEnable ? TRUE : FALSE;
		rasterizerDesc.AntialiasedLineEnable = desc._antialiasedLineEnable ? TRUE : FALSE;
		rasterizerDesc.ForcedSampleCount     = desc._forcedSampleCount;
		rasterizerDesc.ConservativeRaster    = desc._conservativeRasterEnable
			? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON
			: D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		return rasterizerDesc;
	}

	inline D3D12_RENDER_TARGET_BLEND_DESC toD3D12RenderTargetBlendDesc(const RhiRenderTargetBlendDesc& desc)
	{
		D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
		rtBlendDesc.BlendEnable           = desc._blendEnable ? TRUE : FALSE;
		rtBlendDesc.LogicOpEnable         = desc._logicOpEnable ? TRUE : FALSE;
		rtBlendDesc.SrcBlend              = D3D12BlendFactorInfo::getInfo(desc._srcBlend)._blend;
		rtBlendDesc.DestBlend             = D3D12BlendFactorInfo::getInfo(desc._destBlend)._blend;
		rtBlendDesc.BlendOp               = D3D12BlendOpInfo::getInfo(desc._blendOp)._blendOp;
		rtBlendDesc.SrcBlendAlpha         = D3D12BlendFactorInfo::getInfo(desc._srcBlendAlpha)._blend;
		rtBlendDesc.DestBlendAlpha        = D3D12BlendFactorInfo::getInfo(desc._destBlendAlpha)._blend;
		rtBlendDesc.BlendOpAlpha          = D3D12BlendOpInfo::getInfo(desc._blendOpAlpha)._blendOp;
		rtBlendDesc.LogicOp               = D3D12LogicOpInfo::getInfo(desc._logicOp)._logicOp;
		rtBlendDesc.RenderTargetWriteMask = static_cast<UINT8>(desc._renderTargetWriteMask);
		return rtBlendDesc;
	}

	inline D3D12_BLEND_DESC toD3D12BlendDesc(const RhiBlendDesc& desc)
	{
		D3D12_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable  = desc._alphaToCoverageEnable ? TRUE : FALSE;
		blendDesc.IndependentBlendEnable = desc._independentBlendEnable ? TRUE : FALSE;
		for (uint32 idx = 0; idx < RhiMaxRenderTargets; ++idx)
		{
			blendDesc.RenderTarget[idx] = toD3D12RenderTargetBlendDesc(desc._renderTarget[idx]);
		}
		return blendDesc;
	}

	inline D3D12_DEPTH_STENCILOP_DESC toD3D12DepthStencilOpDesc(const RhiDepthStencilOpDesc& desc)
	{
		D3D12_DEPTH_STENCILOP_DESC opDesc = {};
		opDesc.StencilFailOp      = D3D12StencilOpInfo::getInfo(desc._stencilFailOp)._stencilOp;
		opDesc.StencilDepthFailOp = D3D12StencilOpInfo::getInfo(desc._stencilDepthFailOp)._stencilOp;
		opDesc.StencilPassOp      = D3D12StencilOpInfo::getInfo(desc._stencilPassOp)._stencilOp;
		opDesc.StencilFunc        = D3D12ComparisonFunctionInfo::getInfo(desc._stencilFunc)._comparisonFunc;
		return opDesc;
	}

	inline D3D12_DEPTH_STENCIL_DESC toD3D12DepthStencilDesc(const RhiDepthStencilDesc& desc)
	{
		D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable      = desc._depthEnable ? TRUE : FALSE;
		depthStencilDesc.DepthWriteMask   = desc._depthWriteEnable ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc        = D3D12ComparisonFunctionInfo::getInfo(desc._depthFunc)._comparisonFunc;
		depthStencilDesc.StencilEnable    = desc._stencilEnable ? TRUE : FALSE;
		depthStencilDesc.StencilReadMask  = desc._stencilReadMask;
		depthStencilDesc.StencilWriteMask = desc._stencilWriteMask;
		depthStencilDesc.FrontFace        = toD3D12DepthStencilOpDesc(desc._frontFace);
		depthStencilDesc.BackFace         = toD3D12DepthStencilOpDesc(desc._backFace);
		return depthStencilDesc;
	}
}
