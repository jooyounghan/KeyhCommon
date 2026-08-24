#include "RhiSystemPch.h"
#include "D3D12Device.h"
#include "D3D12GraphicsPipeline.h"
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

		KEYH_ASSERT(desc._renderTargetCount <= D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT, "Render target count exceeds D3D12 maximum.");

		D3D12_INPUT_ELEMENT_DESC inputElements[D3D12_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT] = {};
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = toD3D12GraphicsPipelineStateDesc(
			desc,
			inputElements,
			static_cast<uint32>(_countof(inputElements)));

		const HRESULT hr = device->getNativeDevice()->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&_pipelineState));
		KEYH_ASSERT_ARGS(SUCCEEDED(hr), "Failed to create D3D12 graphics pipeline state. HRESULT: 0x%X", hr);
	}
}
