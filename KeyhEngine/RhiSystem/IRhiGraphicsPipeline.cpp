#include "RhiSystemPch.h"
#include "D3D12Device.h"
#include "D3D12GraphicsPipeline.h"
#include "D3D12PipelineLayout.h"
#include "D3D12RhiConvert.h"

namespace keyh
{
	IRhiGraphicsPipeline::IRhiGraphicsPipeline(const RhiGraphicsPipelineDesc& desc)
		: _desc(desc)
	{}

	D3D12GraphicsPipeline::D3D12GraphicsPipeline(D3D12Device* device, const RhiGraphicsPipelineDesc& desc)
		: IRhiGraphicsPipeline(desc)
	{
		if ((desc._renderTargetFormats == nullptr) != (desc._renderTargetCount == 0))
		{
			KEYH_ASSERT(false, "Render target formats and count must be specified together.");
			return;
		}

		const uint32 renderTargetCount = desc._renderTargetCount;
		KEYH_ASSERT(renderTargetCount <= D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT, "Render target count exceeds D3D12 maximum.");

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {};
		pipelineStateDesc.pRootSignature = desc._pipelineLayout != nullptr
			? static_cast<D3D12PipelineLayout*>(desc._pipelineLayout)->getNativeRootSignature()
			: nullptr;
		pipelineStateDesc.VS = { desc._vertexShader._data, desc._vertexShader._sizeInBytes };
		pipelineStateDesc.PS = { desc._pixelShader._data, desc._pixelShader._sizeInBytes };
		pipelineStateDesc.HS = { desc._hullShader._data, desc._hullShader._sizeInBytes };
		pipelineStateDesc.DS = { desc._domainShader._data, desc._domainShader._sizeInBytes };
		pipelineStateDesc.GS = { desc._geometryShader._data, desc._geometryShader._sizeInBytes };
		pipelineStateDesc.BlendState = toD3D12BlendDesc(desc._blendDesc);
		pipelineStateDesc.SampleMask = UINT32_MAX;
		pipelineStateDesc.RasterizerState = toD3D12RasterizerDesc(desc._rasterizerDesc);
		pipelineStateDesc.DepthStencilState = toD3D12DepthStencilDesc(desc._depthStencilDesc);
		pipelineStateDesc.InputLayout = {};
		pipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineStateDesc.PrimitiveTopologyType = D3D12PrimitiveTopologyTypeInfo::getInfo(desc._primitiveTopologyType)._topologyType;
		pipelineStateDesc.NumRenderTargets = renderTargetCount;

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
