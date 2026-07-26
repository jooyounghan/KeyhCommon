#include "RenderRhiPch.h"
#include "IRhiInstance.h"
#include "IRhiAdapter.h"
#include "IRhiDevice.h"

namespace keyh
{
#pragma region D3D12Instance
	bool D3D12Instance::initialize()
	{
		UINT dxgiFactoryFlags = 0;
#ifdef KEYH_DEBUG
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

		HRESULT hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&_factory));
		if (FAILED(hr))
		{
			KEYH_ASSERT(false, "Failed to create DXGI Factory.");
			return false;
		}

		Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
		for (UINT i = 0; _factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter4;
			if (SUCCEEDED(adapter.As(&adapter4)))
			{
				IRhiAdapter* adapter = _adapters.emplace_back<D3D12Adapter>(adapter4);
				IRhiDevice* device = _devices.emplace_back<D3D12Device>();
				device->initialize(adapter);
			}
		}

		return true;
	}
#pragma endregion
}
