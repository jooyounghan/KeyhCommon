#pragma once
#include "IRhiFence.h"

namespace keyh
{
	class D3D12Device;

	class D3D12Fence : public IRhiFence
	{
	public:
		D3D12Fence(D3D12Device* device, const RhiFenceDesc& desc);
		~D3D12Fence() override;

	public:
		virtual void waitForSignal(uint64 timeoutInNanoseconds) override;

	public:
		inline ID3D12Fence* getNativeFence() const
		{
			return _fence.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D12Fence> _fence;
		HANDLE _event = nullptr;
	};
}
