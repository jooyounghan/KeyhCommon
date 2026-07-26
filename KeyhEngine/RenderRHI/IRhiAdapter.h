#pragma once
namespace keyh
{
	struct RhiAdapterInfo
	{
		StaticStringW	description;
		size_t			dedicatedVideoMemory = 0;
		size_t			dedicatedSystemMemory = 0;
		size_t			sharedSystemMemory = 0;
		uint32			vendorId = kInvalidUint32;
		uint32			deviceId = kInvalidUint32;
		bool			isSoftware = false;
	};

	class IRhiAdapter
	{
	public:
		IRhiAdapter() = default;
		virtual ~IRhiAdapter() = default;

	public:
		inline const RhiAdapterInfo& getInfo() const { return _info; }

	protected:
		RhiAdapterInfo _info;
	};

#pragma region D3D12Adapter
	class D3D12Adapter : public IRhiAdapter
	{
	public:
		D3D12Adapter(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter);
		virtual ~D3D12Adapter() = default;

	public:
		inline IDXGIAdapter4* getDxgiAdapter() const { return _adapter.Get(); }

	private:
		Microsoft::WRL::ComPtr<IDXGIAdapter4>	_adapter;
	};
#pragma endregion
}

