#include "RenderRhiPch.h"
#include "IRhiDevice.h"
#include "IRhiAdapter.h"

namespace keyh
{
	bool D3D12Device::initialize(IRhiAdapter* adapter)
	{
		D3D12Adapter* d3d12Adapter = static_cast<D3D12Adapter*>(adapter);

		IDXGIAdapter4* dxgiAdapter = d3d12Adapter ? d3d12Adapter->getDxgiAdapter() : nullptr;
		HRESULT hr = D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&_device));
		return SUCCEEDED(hr);
	}
}
