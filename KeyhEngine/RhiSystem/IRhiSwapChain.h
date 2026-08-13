#pragma once
#include "RhiEnum.h"

namespace keyh
{
	class IRhiTexture;
	class IRhiCommandQueue;
	class D3D12Device;

	struct RHISwapChainDesc
	{
		void*				_windowHandle = nullptr;
		uint32				_width = 0;
		uint32				_height = 0;
		uint32				_bufferCount = 2;
		EResourceFormat		_format = EResourceFormat::R8G8B8A8_UNorm;
		bool				_enableVSync = true;
	};

	class IRhiSwapChain
	{
	public:
		IRhiSwapChain(const RHISwapChainDesc& desc);
		virtual ~IRhiSwapChain() = default;

	protected:
		RHISwapChainDesc		_desc;

	public:
		inline uint32			getBackBufferCount() { return _desc._bufferCount; }

	//public:
	//	virtual void			present() = 0;
	//	virtual void			resize(uint32 width, uint32 height) = 0;
	//	virtual IRhiTexture*	getBackBuffer() = 0;
	};

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

