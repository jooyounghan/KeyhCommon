#pragma once
namespace keyh
{
	template<typename EnumType>
    constexpr uint32 calculateFlagCount(EnumType maxFlag)
    {
        uint32 value = static_cast<uint32>(maxFlag);
        uint32 count = 0;
        while (value > 0)
        {
            value >>= 1;
            count++;
        }
        return count;
    }

	template<typename Derived, uint32 Count>
    struct InfoList
    {
        const Derived* _items[Count] = {};
        uint32 _count = 0;
    };

    template <typename EnumType, typename InfoType, uint32 Count, typename Derived, bool IsFlag = false>
    class IEnumInfoTable;

    template <typename EnumType, typename InfoType, uint32 Count, typename Derived>
    class IEnumInfoTable<EnumType, InfoType, Count, Derived, false>
    {
    public:
        static void initializeTable()
        {
            Derived::initializePlatformTable();
        }
        inline static const Derived& getInfo(EnumType type) { return getTable()[static_cast<uint32_t>(type)]; }

    protected:
        static void registerEntry(EnumType type, Derived info)
        {
            getTable()[static_cast<uint32_t>(type)] = std::move(info);
        }

    private:
        static Derived* getTable()
        {
            static Derived table[Count];
            return table;
        }
    };

    template <typename EnumType, typename InfoType, uint32 Count, typename Derived>
    class IEnumInfoTable<EnumType, InfoType, Count, Derived, true>
    {
    public:
        static void initializeTable()
        {
            Derived::initializePlatformTable();
        }

        inline static const Derived& getInfoByBitIndex(uint32 bitIndex) { return getTable()[bitIndex]; }

        static InfoList<Derived, Count> getInfoList(EnumType combinedFlags);

    protected:
        static void registerEntry(uint32 index, Derived info)
        {
            getTable()[index] = std::move(info);
        }

    private:
        static Derived* getTable()
        {
            static Derived table[static_cast<uint32_t>(Count)];
            return table;
        }
    };

#define UINT(x) static_cast<uint32>(x)
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
        FlyweightStringA    _name;
        unsigned int        _bytesPerPixel;
        unsigned int        _blockSize;
        bool                _isDepthFormat;
        bool                _isCompressed;
        DXGI_FORMAT         _format = DXGI_FORMAT_UNKNOWN;
        DXGI_FORMAT         _srgbFormat = DXGI_FORMAT_UNKNOWN;
    };

    struct D3D12ResourceFormatInfo : public ResourceFormatInfoBase, public IEnumInfoTable<EResourceFormat, ResourceFormatInfoBase, UINT(EResourceFormat::Count), D3D12ResourceFormatInfo>
    {
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
	enum class ECommandQueueType : uint8
	{
		Direct,
		Compute,
		Copy,
		Count
	};

    struct CommandQueueInfoBase
    {
        FlyweightStringA        _name;
        bool                    _canPresent;
        D3D12_COMMAND_LIST_TYPE _type = D3D12_COMMAND_LIST_TYPE_NONE;
    };

    struct D3D12CommandQueueInfo : public CommandQueueInfoBase, public IEnumInfoTable<ECommandQueueType, CommandQueueInfoBase, UINT(ECommandQueueType::Count), D3D12CommandQueueInfo>
    {

        D3D12CommandQueueInfo() = default;
        D3D12CommandQueueInfo(
            const char* name
            , bool canPresent
            , D3D12_COMMAND_LIST_TYPE type
        );

        static void initializePlatformTable();
    };
#pragma endregion

#pragma region HeapType
	enum class EHeapType : uint8
	{
		Default,
		Upload,
		Readback,
		Count
	};

	struct HeapTypeInfoBase
	{
		FlyweightStringA    _name;
		D3D12_HEAP_TYPE     _heapType = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	};

	struct D3D12HeapTypeInfo : public HeapTypeInfoBase, public IEnumInfoTable<EHeapType, HeapTypeInfoBase, UINT(EHeapType::Count), D3D12HeapTypeInfo>
	{
		D3D12HeapTypeInfo() = default;
		D3D12HeapTypeInfo(
			const char* name
			, D3D12_HEAP_TYPE heapType
		);
		static void initializePlatformTable();
	};
#pragma endregion

#pragma region ResourceDimension
	enum class EResourceDimension : uint8
	{
		Unknown,
		Buffer,
		Texture1D,
		Texture2D,
		Texture3D,
		Count
	};

	struct ResourceDimensionInfoBase
	{
		FlyweightStringA    _name;
		D3D12_RESOURCE_DIMENSION _dimension = D3D12_RESOURCE_DIMENSION_UNKNOWN;
	};

	struct D3D12ResourceDimensionInfo : public ResourceDimensionInfoBase, public IEnumInfoTable<EResourceDimension, ResourceDimensionInfoBase, UINT(EResourceDimension::Count), D3D12ResourceDimensionInfo>
	{
		D3D12ResourceDimensionInfo() = default;
		D3D12ResourceDimensionInfo(
			const char* name
			, D3D12_RESOURCE_DIMENSION dimension
		);
		static void initializePlatformTable();
	};
#pragma endregion


#pragma region ResourceFlag
    enum class EResourceFlag : unsigned int
    {
        None = 0,
        DenyShaderResource = 1 << 0,
        RenderTarget = 1 << 1,
        DepthStencil = 1 << 2,
        UnorderedAccess = 1 << 3,
        SimultaneousAccess = 1 << 4,
        CrossAdapter = 1 << 5,
        AccelerationStructure = 1 << 6,

        Max = AccelerationStructure
    };

	struct ResourceFlagInfoBase
    {
        FlyweightStringA    _name;
        D3D12_RESOURCE_FLAGS _flags = D3D12_RESOURCE_FLAG_NONE;
    };

    struct D3D12ResourceFlagInfo : public ResourceFlagInfoBase, public IEnumInfoTable<EResourceFlag, ResourceFlagInfoBase, UINT(EResourceFlag::Max), D3D12ResourceFlagInfo, true>
    {
        D3D12ResourceFlagInfo() = default;
        D3D12ResourceFlagInfo(
            const char* name
            , D3D12_RESOURCE_FLAGS flags
        );
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region ResourceState
	enum class EResourceState : unsigned int
	{
		Common = 0,
		VertexAndConstantBuffer = 1 << 0,
		IndexBuffer = 1 << 1,
		RenderTarget = 1 << 2,
		UnorderedAccess = 1 << 3,
		DepthWrite = 1 << 4,
		DepthRead = 1 << 5,
		NonPixelShaderResource = 1 << 6,
		PixelShaderResource = 1 << 7,
		StreamOut = 1 << 8,
		IndirectArgument = 1 << 9,
		CopyDest = 1 << 10,
		CopySource = 1 << 11,
		ResolveDest = 1 << 12,
		ResolveSource = 1 << 13,
		RaytracingAccelerationStructure = 1 << 14,

        Max = RaytracingAccelerationStructure
	};
	struct ResourceStateInfoBase
	{
		FlyweightStringA    _name;
		D3D12_RESOURCE_STATES _state = D3D12_RESOURCE_STATE_COMMON;
	};
	struct D3D12ResourceStateInfo : public ResourceStateInfoBase, public IEnumInfoTable<EResourceState, ResourceStateInfoBase, UINT(EResourceState::Max), D3D12ResourceStateInfo, true>
	{
		D3D12ResourceStateInfo() = default;
		D3D12ResourceStateInfo(
			const char* name
			, D3D12_RESOURCE_STATES state
		);
		static void initializePlatformTable();
	};
    #undef UINT
}