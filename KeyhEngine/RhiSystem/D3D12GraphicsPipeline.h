#pragma once
#include "IRhiGraphicsPipeline.h"

namespace keyh
{
	class D3D12Device;

	class D3D12GraphicsPipeline : public IRhiGraphicsPipeline
	{
	public:
		D3D12GraphicsPipeline(D3D12Device* device, const RhiGraphicsPipelineDesc& desc);
		~D3D12GraphicsPipeline() override = default;

	public:
		inline ID3D12PipelineState* getNativePipelineState() const
		{
			return _pipelineState.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState;
	};
}
