#pragma once
#include "RhiEnum.h"

namespace keyh
{
	class IRhiTexture;
	class IRhiCommandQueue;

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
		IRhiSwapChain(RHISwapChainDesc desc);
		virtual ~IRhiSwapChain() = default;

	protected:
		uint32					_width;
		uint32					_height;
		uint32					_bufferCount;
		EResourceFormat			_format;

	public:
		virtual void			present() = 0;
		virtual void			resize(uint32 width, uint32 height) = 0;
		virtual IRhiTexture*	getBackBuffer() = 0;
		virtual uint32			getBackBufferCount() = 0;
	};

	class D3D12SwapChain : public IRhiSwapChain
	{
	public:
		D3D12SwapChain(RHISwapChainDesc desc);
		virtual ~D3D12SwapChain() override = default;

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain3> _swapChain;

	public:
		bool initialize(IDXGIFactory7* factory, IRhiCommandQueue* presentQueue);
	};
}

