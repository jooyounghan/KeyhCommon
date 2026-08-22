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
		IRhiSwapChain(const RHISwapChainDesc& desc);
		virtual ~IRhiSwapChain() = default;

	protected:
		RHISwapChainDesc		_desc;

	public:
		inline uint32			getBackBufferCount() { return _desc._bufferCount; }

	public:
		virtual void			present() = 0;
		virtual void			resize(uint32 width, uint32 height) = 0;
		virtual IRhiTexture*	getBackBuffer(uint32 index) = 0;
	};
}

