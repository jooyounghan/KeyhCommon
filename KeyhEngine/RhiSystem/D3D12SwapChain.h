#pragma once
#include "Ptr.h"
#include "IRhiSwapChain.h"

namespace keyh
{
	class D3D12Device;

	class D3D12SwapChain : public IRhiSwapChain
	{
	public:
		D3D12SwapChain(const RHISwapChainDesc& desc, D3D12Device* device, IRhiCommandQueue* presentQueue);
		virtual ~D3D12SwapChain() override = default;

	public:
		virtual void			present() override;
		virtual void			resize(uint32 width, uint32 height) override;
		virtual IRhiTexture*	getBackBuffer(uint32 index) override;

	public:
		inline uint32				getCurrentBackBufferIndex() const { return _swapChain->GetCurrentBackBufferIndex(); }

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain3>	_swapChain;
		OwnerVector<IRhiTexture>				_backBuffers;

		bool initialize(D3D12Device* device, IRhiCommandQueue* presentQueue);
	};
}
