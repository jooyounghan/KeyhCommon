#pragma once
namespace keyh
{
	class IRhiTexture;

	class IRhiSwapChain
	{
	public:
		IRhiSwapChain() = default;
		virtual ~IRhiSwapChain() = default;

	public:
		virtual void			present() = 0;
		virtual void			resize(uint32 width, uint32 height) = 0;
		virtual IRhiTexture*	getBackBuffer() = 0;
		virtual uint32			getBackBufferCount() = 0;
	};

	class D3D12SwapChain : public IRhiSwapChain
	{

	};
}

