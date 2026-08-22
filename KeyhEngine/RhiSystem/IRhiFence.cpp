#include "RhiSystemPch.h"
#include "D3D12Device.h"
#include "D3D12Fence.h"

namespace keyh
{
	IRhiFence::IRhiFence(const RhiFenceDesc& desc)
		: _desc(desc)
	{}

	D3D12Fence::D3D12Fence(D3D12Device* device, const RhiFenceDesc& desc)
		: IRhiFence(desc)
	{
		const HRESULT hr = device->getNativeDevice()->CreateFence(desc._initialValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
		KEYH_ASSERT_ARGS(SUCCEEDED(hr), "Failed to create D3D12 fence. HRESULT: 0x%X", hr);

		_event = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
		KEYH_ASSERT(_event != nullptr, "Failed to create D3D12 fence event.");
	}

	D3D12Fence::~D3D12Fence()
	{
		if (_event != nullptr)
		{
			::CloseHandle(_event);
			_event = nullptr;
		}
	}

	void D3D12Fence::waitForSignal(uint64 timeoutInNanoseconds)
	{
		if (_fence == nullptr)
		{
			return;
		}

		if (_fence->GetCompletedValue() >= _desc._initialValue)
		{
			return;
		}

		const HRESULT hr = _fence->SetEventOnCompletion(_desc._initialValue, _event);
		KEYH_ASSERT_ARGS(SUCCEEDED(hr), "Failed to register D3D12 fence completion event. HRESULT: 0x%X", hr);
		if (FAILED(hr))
		{
			return;
		}

		DWORD timeoutInMilliseconds = INFINITE;
		if (timeoutInNanoseconds != kInvalidUint64)
		{
			const uint64 timeoutInMilliseconds64 = (timeoutInNanoseconds + 999999ULL) / 1000000ULL;
			timeoutInMilliseconds = timeoutInMilliseconds64 > static_cast<uint64>(UINT32_MAX)
				? INFINITE
				: static_cast<DWORD>(timeoutInMilliseconds64);
		}

		const DWORD waitResult = ::WaitForSingleObject(_event, timeoutInMilliseconds);
		KEYH_ASSERT_ARGS(waitResult == WAIT_OBJECT_0, "Failed to wait for D3D12 fence signal. Result: 0x%X", waitResult);
	}
}
