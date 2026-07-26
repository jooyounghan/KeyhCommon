#pragma once
namespace keyh
{
	class IRhiDevice;
	class IRhiAdapter;

	class IRhiInstance
	{
	public:
		IRhiInstance() = default;
		virtual ~IRhiInstance() = default;

	public:
		virtual bool initialize() = 0;

	public:
		inline const OwnerVector<IRhiAdapter>&	getAdapters() const { return _adapters; }
		inline const OwnerVector<IRhiDevice>&	getDevices() const { return _devices; }

	protected:
		OwnerVector<IRhiAdapter>	_adapters;
		OwnerVector<IRhiDevice>		_devices;
	};

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

