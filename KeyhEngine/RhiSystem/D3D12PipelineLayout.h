#pragma once
#include "IRhiPipelineLayout.h"

namespace keyh
{
	class D3D12Device;

	class D3D12PipelineLayout : public IRhiPipelineLayout
	{
	public:
		D3D12PipelineLayout(D3D12Device* device, const RhiPipelineLayoutDesc& desc);
		~D3D12PipelineLayout() override = default;

	public:
		inline ID3D12RootSignature* getNativeRootSignature() const
		{
			return _rootSignature.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature;
	};
}
