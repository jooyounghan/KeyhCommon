#include "RenderRhiPch.h"
#include "IRhiAdapter.h"

namespace keyh
{
#pragma region D3D12Adapter
	D3D12Adapter::D3D12Adapter(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter)
		: _adapter(adapter)
	{
		DXGI_ADAPTER_DESC1 desc;
		HRESULT hr = _adapter->GetDesc1(&desc);
		if (FAILED(hr))
		{
			KEYH_ASSERT(false, "Failed to get adapter description.");
			return;
		}
		_info.description = desc.Description;
		_info.dedicatedVideoMemory = desc.DedicatedVideoMemory;
		_info.dedicatedSystemMemory = desc.DedicatedSystemMemory;
		_info.sharedSystemMemory = desc.SharedSystemMemory;
		_info.vendorId = desc.VendorId;
		_info.deviceId = desc.DeviceId;
		_info.isSoftware = (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0;
	}
#pragma endregion
}
