#pragma once
#include "IRhiComputePipeline.h"

namespace keyh
{
	class D3D12Device;

	class D3D12ComputePipeline : public IRhiComputePipeline
	{
	public:
		D3D12ComputePipeline(D3D12Device* device, const RhiComputePipelineDesc& desc);
		~D3D12ComputePipeline() override = default;

	public:
		inline ID3D12PipelineState* getNativePipelineState() const
		{
			return _pipelineState.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState;
	};
}
