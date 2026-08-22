#include "RhiSystemPch.h"
#include "D3D12Device.h"
#include "D3D12GraphicsPipeline.h"
#include "D3D12PipelineLayout.h"
#include "D3D12RhiConvert.h"

namespace keyh
{
	namespace
	{
		D3D12_RASTERIZER_DESC createDefaultRasterizerDesc()
		{
			D3D12_RASTERIZER_DESC desc = {};
			desc.FillMode = D3D12_FILL_MODE_SOLID;
			desc.CullMode = D3D12_CULL_MODE_BACK;
			desc.FrontCounterClockwise = FALSE;
			desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
			desc.DepthClipEnable = TRUE;
			desc.MultisampleEnable = FALSE;
			desc.AntialiasedLineEnable = FALSE;
			desc.ForcedSampleCount = 0;
			desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			return desc;
		}

		D3D12_BLEND_DESC createDefaultBlendDesc()
		{
			D3D12_BLEND_DESC desc = {};
			desc.AlphaToCoverageEnable = FALSE;
			desc.IndependentBlendEnable = FALSE;

			D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
			renderTargetBlendDesc.BlendEnable = FALSE;
			renderTargetBlendDesc.LogicOpEnable = FALSE;
			renderTargetBlendDesc.SrcBlend = D3D12_BLEND_ONE;
			renderTargetBlendDesc.DestBlend = D3D12_BLEND_ZERO;
			renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
			renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
			renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
			renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			renderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
			renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			for (uint32 idx = 0; idx < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++idx)
			{
				desc.RenderTarget[idx] = renderTargetBlendDesc;
			}

			return desc;
		}

		D3D12_DEPTH_STENCIL_DESC createDepthStencilDesc(const RhiGraphicsPipelineDesc& desc)
		{
			D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
			depthStencilDesc.DepthEnable = desc._enableDepthTest || desc._enableDepthWrite;
			depthStencilDesc.DepthWriteMask = desc._enableDepthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
			depthStencilDesc.DepthFunc = D3D12ComparisonFunctionInfo::getInfo(desc._depthComparisonFunc)._comparisonFunc;
			depthStencilDesc.StencilEnable = FALSE;
			depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
			depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
			depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			depthStencilDesc.BackFace = depthStencilDesc.FrontFace;
			return depthStencilDesc;
		}
	}

	IRhiGraphicsPipeline::IRhiGraphicsPipeline(const RhiGraphicsPipelineDesc& desc)
		: _desc(desc)
	{}

	D3D12GraphicsPipeline::D3D12GraphicsPipeline(D3D12Device* device, const RhiGraphicsPipelineDesc& desc)
		: IRhiGraphicsPipeline(desc)
	{
		const uint32 renderTargetCount = desc._renderTargetFormats == nullptr ? 0 : desc._renderTargetCount;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {};
		pipelineStateDesc.pRootSignature = desc._pipelineLayout != nullptr
			? static_cast<D3D12PipelineLayout*>(desc._pipelineLayout)->getNativeRootSignature()
			: nullptr;
		pipelineStateDesc.VS = { desc._vertexShader._data, desc._vertexShader._sizeInBytes };
		pipelineStateDesc.PS = { desc._pixelShader._data, desc._pixelShader._sizeInBytes };
		pipelineStateDesc.BlendState = createDefaultBlendDesc();
		pipelineStateDesc.SampleMask = UINT32_MAX;
		pipelineStateDesc.RasterizerState = createDefaultRasterizerDesc();
		pipelineStateDesc.DepthStencilState = createDepthStencilDesc(desc);
		pipelineStateDesc.InputLayout = {};
		pipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateDesc.NumRenderTargets = renderTargetCount > D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT
			? D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT
			: renderTargetCount;

		for (uint32 idx = 0; idx < pipelineStateDesc.NumRenderTargets; ++idx)
		{
			pipelineStateDesc.RTVFormats[idx] = D3D12ResourceFormatInfo::getInfo(desc._renderTargetFormats[idx])._format;
		}

		pipelineStateDesc.DSVFormat = D3D12ResourceFormatInfo::getInfo(desc._depthStencilFormat)._format;
		pipelineStateDesc.SampleDesc.Count = desc._sampleCount == 0 ? 1 : desc._sampleCount;
		pipelineStateDesc.SampleDesc.Quality = 0;
		pipelineStateDesc.NodeMask = 0;
		pipelineStateDesc.CachedPSO = {};
		pipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		const HRESULT hr = device->getNativeDevice()->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&_pipelineState));
		KEYH_ASSERT_ARGS(SUCCEEDED(hr), "Failed to create D3D12 graphics pipeline state. HRESULT: 0x%X", hr);
	}
}
