#pragma once
#include "IRhiSwapChain.h"

namespace keyh
{
	class D3D12Device;

	class D3D12SwapChain : public IRhiSwapChain
	{
	public:
		D3D12SwapChain(const RHISwapChainDesc& desc, D3D12Device* device, IRhiCommandQueue* presentQueue);
		virtual ~D3D12SwapChain() override = default;

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain3> _swapChain;

		bool initialize(D3D12Device* device, IRhiCommandQueue* presentQueue);
	};
}
