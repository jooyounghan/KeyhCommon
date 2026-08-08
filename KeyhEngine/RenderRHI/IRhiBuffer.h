#pragma once

namespace keyh
{
	struct RhiBufferDesc
	{
		uint32 _width = 0;
		uint32 _height = 0;
		uint32 _depth = 1;
		uint32 _stride = 0;
		EResourceFormat _format = EResourceFormat::Unknown;
		EResourceDimension _dimension = EResourceDimension::Buffer;
		EResourceFlag _flags = EResourceFlag::None;

#if defined(KEYH_PLATFORM_WINDOWS)
		D3D12_RESOURCE_DESC getD3D12ResourceDesc() const;
#endif
	};

	class IRhiBuffer
	{
	public:
		IRhiBuffer(const RhiBufferDesc& desc);
		virtual ~IRhiBuffer() = default;

	public:
		virtual void* map() = 0;
		virtual void unmap() = 0;

	public:
		inline const RhiBufferDesc& getDesc() const { return _desc; }

	protected:
		RhiBufferDesc _desc;
	};

	class D3D12Buffer : public IRhiBuffer
	{
	public:
		D3D12Buffer(ID3D12Device* device, const RhiBufferDesc& desc);
		~D3D12Buffer() override = default;

	public:
		virtual void* map() override;
		virtual void unmap() override;

	public:
		inline ID3D12Resource* getNativeResource() const
		{
			return _resource.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> _resource;
	};
}

