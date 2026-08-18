#pragma once
#include "IRhiInstance.h"

namespace keyh
{
#pragma region D3D12Instance
	class D3D12Instance : public IRhiInstance
	{
	public:
		D3D12Instance() = default;
		virtual ~D3D12Instance() override = default;

	public:
		virtual bool initialize() override;

	private:
		Microsoft::WRL::ComPtr<IDXGIFactory7>	_factory;
	};
#pragma endregion
}
