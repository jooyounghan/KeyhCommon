#pragma once
namespace keyh
{
    enum class EResourceFormat
    {
        Unknown,
        R32G32B32A32_Float,
        B8G8R8A8_UNorm,
        R8G8B8A8_UNorm,
        R32_Float,
        Count
    };

    struct ResourceFormatInfoBase
    {
        FlyweightStringA    _name;
        unsigned int        _bytesPerPixel;
        unsigned int        _blockSize;
        bool                _isDepthFormat;
        bool                _isCompressed;
    };

    template <typename Derived>
    class IResourceFormatTable
    {
    public:
        static void initializeTable()
        {
            Derived::initializePlatformTable();
        }

        static const Derived& getFormatInfo(EResourceFormat format)
        {
            return _table[static_cast<uint32>(format)];
        }

    protected:
        static Derived _table[static_cast<uint32>(EResourceFormat::Count)];
    };

    template <typename Derived>
    Derived IResourceFormatTable<Derived>::_table[static_cast<uint32>(EResourceFormat::Count)];

    struct D3D12ResourceFormatInfo : public ResourceFormatInfoBase, public IResourceFormatTable<D3D12ResourceFormatInfo>
    {
        DXGI_FORMAT _format;
        DXGI_FORMAT _srgbFormat;

        D3D12ResourceFormatInfo() = default;
        D3D12ResourceFormatInfo(
            const char* name
            , unsigned int bytesPerPixel
            , unsigned int blockSize
            , bool isDepthFormat
            , bool isCompressed
            , DXGI_FORMAT format
            , DXGI_FORMAT srgbFormat
        );

        static void initializePlatformTable();
    };
}
