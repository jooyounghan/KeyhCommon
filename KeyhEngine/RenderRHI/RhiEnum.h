#pragma once
namespace keyh
{
    template <typename EnumType, typename InfoType, EnumType Count, typename Derived>
    class IEnumInfoTable
    {
    public:
        static void initializeTable()
        {
            Derived::initializePlatformTable();
        }

        static const Derived& getInfo(EnumType type)
        {
            return _table[static_cast<uint32_t>(type)];
        }

    protected:
        static Derived _table[static_cast<uint32_t>(Count)];
    };

    template <typename EnumType, typename InfoType, EnumType Count, typename Derived>
    Derived IEnumInfoTable<EnumType, InfoType, Count, Derived>::_table[static_cast<uint32_t>(Count)];

#pragma region ResourceFormat
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
        FlyweightStringA _name;
        unsigned int _bytesPerPixel;
        unsigned int _blockSize;
        bool _isDepthFormat;
        bool _isCompressed;
    };

    struct D3D12ResourceFormatInfo : public ResourceFormatInfoBase, public IEnumInfoTable<EResourceFormat, ResourceFormatInfoBase, EResourceFormat::Count, D3D12ResourceFormatInfo>
    {
        DXGI_FORMAT _format;
        DXGI_FORMAT _srgbFormat;

		D3D12ResourceFormatInfo() = default;
        D3D12ResourceFormatInfo(
            const char* name,
            unsigned int bytesPerPixel,
            unsigned int blockSize,
            bool isDepthFormat,
            bool isCompressed,
            DXGI_FORMAT format,
            DXGI_FORMAT srgbFormat
        );

        static void initializePlatformTable();
    };
#pragma endregion

#pragma region CommandQueueType
	enum class ECommandQueueType
	{
		Direct,
		Compute,
		Copy,
		Count
	};

    struct CommandQueueInfoBase
    {
        FlyweightStringA _name;
        bool _canPresent;
    };

    struct D3D12CommandQueueInfo : public CommandQueueInfoBase, public IEnumInfoTable<ECommandQueueType, CommandQueueInfoBase, ECommandQueueType::Count, D3D12CommandQueueInfo>
    {
        D3D12_COMMAND_LIST_TYPE _type;

        D3D12CommandQueueInfo() = default;
        D3D12CommandQueueInfo(
            const char* name
            , bool canPresent
            , D3D12_COMMAND_LIST_TYPE type
        );

        static void initializePlatformTable();
    };
#pragma endregion
}
