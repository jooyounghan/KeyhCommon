#include "RhiSystemPch.h"
#include "D3D12SwapChain.h"
#include "D3D12Device.h"

namespace keyh
{
	IRhiSwapChain::IRhiSwapChain(const RHISwapChainDesc& desc)
		: _desc(desc)
	{
	}


	D3D12SwapChain::D3D12SwapChain(const RHISwapChainDesc& desc, D3D12Device* device, IRhiCommandQueue* presentQueue)
		: IRhiSwapChain(desc)
	{
		initialize(device, presentQueue);
	}

	bool D3D12SwapChain::initialize(D3D12Device* device, IRhiCommandQueue* presentQueue)
	{
		const D3D12ResourceFormatInfo& formatInfo = D3D12ResourceFormatInfo::getInfo(_desc._format);

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = _desc._width;
		swapChainDesc.Height = _desc._height;
		swapChainDesc.Format = formatInfo._format;
		swapChainDesc.Stereo = FALSE;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = _desc._bufferCount;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Flags = 0;

		IDXGIFactory7* factory = device->getDxgiFactory();
		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
		HRESULT hr = factory->CreateSwapChainForHwnd(
			nullptr,
			static_cast<HWND>(_desc._windowHandle),
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain1
		);

		if (FAILED(hr))
		{
			KEYH_ASSERT_ARGS(false, "Failed to create swap chain. HRESULT: 0x%X", hr);
			return false;
		}

		hr = swapChain1.As(&_swapChain);
		if (FAILED(hr))
		{
			KEYH_ASSERT_ARGS(false, "Failed to query IDXGISwapChain3 interface. HRESULT: 0x%X", hr);
			return false;
		}

		return true;
	}
}