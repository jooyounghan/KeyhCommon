#include "RhiSystemPch.h"
#include "D3D12ComputePipeline.h"
#include "D3D12Device.h"
#include "D3D12PipelineLayout.h"

namespace keyh
{
	D3D12ComputePipeline::D3D12ComputePipeline(D3D12Device* device, const RhiComputePipelineDesc& desc)
		: IRhiComputePipeline(desc)
	{
		D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineStateDesc = {};
		pipelineStateDesc.pRootSignature = desc._pipelineLayout != nullptr
			? static_cast<D3D12PipelineLayout*>(desc._pipelineLayout)->getNativeRootSignature()
			: nullptr;
		pipelineStateDesc.CS = { desc._computeShader._data, desc._computeShader._sizeInBytes };
		pipelineStateDesc.NodeMask = 0;
		pipelineStateDesc.CachedPSO = {};
		pipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		const HRESULT hr = device->getNativeDevice()->CreateComputePipelineState(&pipelineStateDesc, IID_PPV_ARGS(&_pipelineState));
		KEYH_ASSERT_ARGS(SUCCEEDED(hr), "Failed to create D3D12 compute pipeline state. HRESULT: 0x%X", hr);
	}
}
