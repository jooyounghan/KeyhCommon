#include "RhiSystemPch.h"
#include "D3D12SwapChain.h"
#include "D3D12Device.h"
#include "D3D12Texture.h"
#include "D3D12RhiEnum.h"
#include "D3D12RhiConvert.h"

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
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = toDXGISwapChainDesc1(_desc);

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

		_backBuffers.reserve(_desc._bufferCount);
		for (uint32 i = 0; i < _desc._bufferCount; ++i)
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> backBufferResource;
			hr = _swapChain->GetBuffer(i, IID_PPV_ARGS(&backBufferResource));
			KEYH_ASSERT_ARGS(SUCCEEDED(hr), "Failed to get swap chain back buffer %u. HRESULT: 0x%X", i, hr);

			_backBuffers.emplace_back(new D3D12Texture(toRhiTextureDesc(_desc), keyh::move(backBufferResource)));
		}

		return true;
	}

	void D3D12SwapChain::present()
	{
		const UINT syncInterval = _desc._enableVSync ? 1 : 0;
		const HRESULT hr = _swapChain->Present(syncInterval, 0);
		KEYH_ASSERT_ARGS(SUCCEEDED(hr), "SwapChain Present failed. HRESULT: 0x%X", hr);
	}

	void D3D12SwapChain::resize(uint32 width, uint32 height)
	{
		_backBuffers.clear();

		_desc._width  = width;
		_desc._height = height;

		const D3D12ResourceFormatInfo& formatInfo = D3D12ResourceFormatInfo::getInfo(_desc._format);
		HRESULT hr = _swapChain->ResizeBuffers(_desc._bufferCount, width, height, formatInfo._format, 0);
		KEYH_ASSERT_ARGS(SUCCEEDED(hr), "SwapChain ResizeBuffers failed. HRESULT: 0x%X", hr);

		_backBuffers.reserve(_desc._bufferCount);
		for (uint32 i = 0; i < _desc._bufferCount; ++i)
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> backBufferResource;
			hr = _swapChain->GetBuffer(i, IID_PPV_ARGS(&backBufferResource));
			KEYH_ASSERT_ARGS(SUCCEEDED(hr), "Failed to get swap chain back buffer %u after resize. HRESULT: 0x%X", i, hr);

			_backBuffers.emplace_back(new D3D12Texture(toRhiTextureDesc(_desc), keyh::move(backBufferResource)));
		}
	}

	IRhiTexture* D3D12SwapChain::getBackBuffer(uint32 index)
	{
		KEYH_ASSERT(index < _backBuffers.size(), "Back buffer index out of range.");
		return _backBuffers[index].get();
	}
}