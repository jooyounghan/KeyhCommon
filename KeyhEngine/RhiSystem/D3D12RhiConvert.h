#pragma once
// Internal D3D12 conversion helpers — not part of the public RHI interface.
// Include only from D3D12 implementation .cpp files.
#include "IRhiBuffer.h"
#include "IRhiTexture.h"
#include "IRhiSampler.h"
#include "IRhiGraphicsPipeline.h"
#include "IRhiSwapChain.h"
#include "RhiResourceBarrier.h"
#include "D3D12Buffer.h"
#include "D3D12Texture.h"
#include "D3D12PipelineLayout.h"

namespace keyh
{
	inline D3D12_SHADER_BYTECODE toD3D12ShaderBytecode(const RhiShaderBytecode& desc)
	{
		D3D12_SHADER_BYTECODE shaderBytecode = {};
		shaderBytecode.pShaderBytecode = desc._data;
		shaderBytecode.BytecodeLength = desc._sizeInBytes;
		return shaderBytecode;
	}

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

	inline D3D12_INPUT_CLASSIFICATION toD3D12InputClassification(EVertexInputRate inputRate)
	{
		switch (inputRate)
		{
		case EVertexInputRate::PerVertex:
			return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		case EVertexInputRate::PerInstance:
			return D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
		default:
			KEYH_ASSERT(false, "Unsupported vertex input rate.");
			return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		}
	}

	inline const RhiInputBindingDesc* findRhiInputBindingDesc(const RhiInputLayoutDesc& desc, uint32 binding)
	{
		for (uint32 idx = 0; idx < desc._bindingCount; ++idx)
		{
			const RhiInputBindingDesc& inputBinding = desc._bindings[idx];
			KEYH_ASSERT(inputBinding._binding < D3D12_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, "Input binding index exceeds D3D12 maximum.");
			if (inputBinding._binding == binding)
			{
				return &inputBinding;
			}
		}

		return nullptr;
	}

	inline D3D12_INPUT_LAYOUT_DESC toD3D12InputLayoutDesc(
		const RhiInputLayoutDesc& desc,
		D3D12_INPUT_ELEMENT_DESC* outInputElements,
		uint32 maxInputElementCount)
	{
		KEYH_ASSERT((desc._bindings == nullptr) == (desc._bindingCount == 0), "Input bindings and count must be specified together.");
		KEYH_ASSERT((desc._attributes == nullptr) == (desc._attributeCount == 0), "Input attributes and count must be specified together.");
		KEYH_ASSERT(desc._bindingCount <= D3D12_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, "Input binding count exceeds D3D12 maximum.");
		KEYH_ASSERT(desc._attributeCount <= maxInputElementCount, "Input attribute count exceeds the provided D3D12 input element capacity.");
		KEYH_ASSERT(desc._attributeCount == 0 || outInputElements != nullptr, "D3D12 input element storage must be provided when input attributes are present.");

		static constexpr const char* kD3D12InputSemanticName = "ATTRIBUTE";

		for (uint32 idx = 0; idx < desc._bindingCount; ++idx)
		{
			const RhiInputBindingDesc& inputBinding = desc._bindings[idx];
			KEYH_ASSERT(inputBinding._strideInBytes > 0, "Input bindings must specify a non-zero stride.");
		}

		for (uint32 idx = 0; idx < desc._attributeCount; ++idx)
		{
			const RhiInputAttributeDesc& inputAttribute = desc._attributes[idx];
			const RhiInputBindingDesc* inputBinding = findRhiInputBindingDesc(desc, inputAttribute._binding);

			KEYH_ASSERT(inputBinding != nullptr, "Each input attribute must reference a valid input binding.");
			KEYH_ASSERT(inputAttribute._format != EResourceFormat::Unknown, "Input attributes must use a concrete resource format.");

			D3D12_INPUT_ELEMENT_DESC& inputElement = outInputElements[idx];
			inputElement.SemanticName         = kD3D12InputSemanticName;
			inputElement.SemanticIndex        = inputAttribute._location;
			inputElement.Format               = D3D12ResourceFormatInfo::getInfo(inputAttribute._format)._format;
			inputElement.InputSlot            = inputAttribute._binding;
			inputElement.AlignedByteOffset    = inputAttribute._offsetInBytes;
			inputElement.InputSlotClass       = toD3D12InputClassification(inputBinding != nullptr ? inputBinding->_inputRate : EVertexInputRate::PerVertex);
			inputElement.InstanceDataStepRate = inputBinding != nullptr && inputBinding->_inputRate == EVertexInputRate::PerInstance
				? (inputBinding->_instanceStepRate == 0 ? 1u : inputBinding->_instanceStepRate)
				: 0;
		}

		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
		inputLayoutDesc.pInputElementDescs = desc._attributeCount > 0 ? outInputElements : nullptr;
		inputLayoutDesc.NumElements        = desc._attributeCount;
		return inputLayoutDesc;
	}

	inline D3D12_VERTEX_BUFFER_VIEW toD3D12VertexBufferView(const RhiVertexBufferView& view)
	{
		D3D12_VERTEX_BUFFER_VIEW d3dView = {};
		d3dView.BufferLocation = static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(view._bufferLocation);
		d3dView.SizeInBytes    = view._sizeInBytes;
		d3dView.StrideInBytes  = view._strideInBytes;
		return d3dView;
	}

	inline D3D12_RESOURCE_BARRIER toD3D12ResourceBarrier(const RhiResourceBarrier& barrier)
	{
		KEYH_ASSERT(barrier._type == EResourceBarrierType::Transition, "Only Transition barriers are currently supported.");

		ID3D12Resource* pResource = nullptr;
		if (barrier._transition._target == EResourceBarrierTarget::Buffer && barrier._transition._buffer != nullptr)
		{
			pResource = static_cast<D3D12Buffer*>(barrier._transition._buffer)->getNativeResource();
		}
		else if (barrier._transition._target == EResourceBarrierTarget::Texture && barrier._transition._texture != nullptr)
		{
			pResource = static_cast<D3D12Texture*>(barrier._transition._texture)->getNativeResource();
		}

		KEYH_ASSERT(pResource != nullptr, "Resource barrier target must have a valid native resource.");

		D3D12_RESOURCE_BARRIER d3dBarrier               = {};
		d3dBarrier.Type                                  = D3D12ResourceBarrierTypeInfo::getInfo(barrier._type)._barrierType;
		d3dBarrier.Flags                                 = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		d3dBarrier.Transition.pResource                  = pResource;
		d3dBarrier.Transition.StateBefore                = toD3D12ResourceStates(barrier._transition._stateBefore);
		d3dBarrier.Transition.StateAfter                 = toD3D12ResourceStates(barrier._transition._stateAfter);
		d3dBarrier.Transition.Subresource                = barrier._transition._subresource;
		return d3dBarrier;
	}

	inline DXGI_SWAP_CHAIN_DESC1 toDXGISwapChainDesc1(const RHISwapChainDesc& desc)
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc       = {};
		swapChainDesc.Width                        = desc._width;
		swapChainDesc.Height                       = desc._height;
		swapChainDesc.Format                       = D3D12ResourceFormatInfo::getInfo(desc._format)._format;
		swapChainDesc.Stereo                       = FALSE;
		swapChainDesc.SampleDesc.Count             = 1;
		swapChainDesc.SampleDesc.Quality           = 0;
		swapChainDesc.BufferUsage                  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount                  = desc._bufferCount;
		swapChainDesc.Scaling                      = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect                   = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode                    = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Flags                        = 0;
		return swapChainDesc;
	}

	inline RhiTextureDesc toRhiTextureDesc(const RHISwapChainDesc& desc)
	{
		RhiTextureDesc textureDesc;
		textureDesc._width              = desc._width;
		textureDesc._height             = desc._height;
		textureDesc._format             = desc._format;
		textureDesc._dimension          = EResourceDimension::Texture2D;
		textureDesc._resourceFlags      = EResourceFlag::RenderTarget;
		textureDesc._resourceStateFlags = EResourceState::Common;
		return textureDesc;
	}

	inline D3D12_GRAPHICS_PIPELINE_STATE_DESC toD3D12GraphicsPipelineStateDesc(
		const RhiGraphicsPipelineDesc& desc,
		D3D12_INPUT_ELEMENT_DESC* outInputElements,
		uint32 maxInputElementCount)
	{
		const uint32 renderTargetCount = desc._renderTargetCount;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.pRootSignature = desc._pipelineLayout != nullptr
			? static_cast<D3D12PipelineLayout*>(desc._pipelineLayout)->getNativeRootSignature()
			: nullptr;
		psoDesc.VS                    = toD3D12ShaderBytecode(desc._vertexShader);
		psoDesc.PS                    = toD3D12ShaderBytecode(desc._pixelShader);
		psoDesc.HS                    = toD3D12ShaderBytecode(desc._hullShader);
		psoDesc.DS                    = toD3D12ShaderBytecode(desc._domainShader);
		psoDesc.GS                    = toD3D12ShaderBytecode(desc._geometryShader);
		psoDesc.BlendState            = toD3D12BlendDesc(desc._blendDesc);
		psoDesc.SampleMask            = UINT32_MAX;
		psoDesc.RasterizerState       = toD3D12RasterizerDesc(desc._rasterizerDesc);
		psoDesc.DepthStencilState     = toD3D12DepthStencilDesc(desc._depthStencilDesc);
		psoDesc.InputLayout           = toD3D12InputLayoutDesc(desc._inputLayout, outInputElements, maxInputElementCount);
		psoDesc.IBStripCutValue       = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		psoDesc.PrimitiveTopologyType = D3D12PrimitiveTopologyTypeInfo::getInfo(desc._primitiveTopologyType)._topologyType;
		psoDesc.NumRenderTargets      = renderTargetCount;

		if (desc._renderTargetFormats != nullptr)
		{
			for (uint32 idx = 0; idx < renderTargetCount; ++idx)
			{
				psoDesc.RTVFormats[idx] = D3D12ResourceFormatInfo::getInfo(desc._renderTargetFormats[idx])._format;
			}
		}
		else
		{
			KEYH_ASSERT(renderTargetCount == 0, "Render target formats must be provided when render target count is non-zero.");
		}

		psoDesc.DSVFormat          = D3D12ResourceFormatInfo::getInfo(desc._depthStencilFormat)._format;
		psoDesc.SampleDesc.Count   = desc._sampleCount == 0 ? 1 : desc._sampleCount;
		psoDesc.SampleDesc.Quality = 0;
		psoDesc.NodeMask           = 0;
		psoDesc.CachedPSO          = {};
		psoDesc.Flags              = D3D12_PIPELINE_STATE_FLAG_NONE;

		return psoDesc;
	}
}
