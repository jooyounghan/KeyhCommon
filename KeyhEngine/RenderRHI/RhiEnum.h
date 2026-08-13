#pragma once
namespace keyh
{
    template<typename EnumType>
    constexpr uint32 calculateFlagCount(EnumType maxFlag)
    {
        uint32 value = static_cast<uint32>(maxFlag);
        uint32 bitCount = 0;

        while (value)
        {
            value >>= 1;
            ++bitCount;
        }

        return bitCount + 1;
    }

    template<typename EnumType>
    constexpr uint32 getIndex(EnumType value)
    {
        uint32 v = static_cast<uint32>(value);

        if (v == 0)
            return 0;

        uint32 index = 1;
        while (v >>= 1)
            ++index;

        return index;
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
            getTable()[static_cast<uint32_t>(type)] = keyh::move(info);
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
        static void registerEntry(EnumType type, Derived info)
        {
            getTable()[getIndex(type)] = keyh::move(info);
        }

    private:
        static Derived* getTable()
        {
            static Derived table[Count];
            return table;
        }
    };

#define EnumTable(EnumType, InfoType, EnumCount, Derived)   \
    IEnumInfoTable<EnumType, InfoType, static_cast<uint32>(EnumCount), Derived>

#define EnumFlagTable(EnumType, InfoType, EnumMax, Derived)   \
    IEnumInfoTable<EnumType, InfoType, calculateFlagCount(EnumMax), Derived, true>

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

    struct D3D12ResourceFormatInfo : public ResourceFormatInfoBase, public EnumTable(EResourceFormat, ResourceFormatInfoBase, EResourceFormat::Count, D3D12ResourceFormatInfo)
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

    struct D3D12CommandQueueInfo : public CommandQueueInfoBase, public EnumTable(ECommandQueueType, CommandQueueInfoBase, ECommandQueueType::Count, D3D12CommandQueueInfo)
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

	struct D3D12HeapTypeInfo : public HeapTypeInfoBase, public EnumTable(EHeapType, HeapTypeInfoBase, EHeapType::Count, D3D12HeapTypeInfo)
	{
		D3D12HeapTypeInfo() = default;
		D3D12HeapTypeInfo(
			const char* name
			, D3D12_HEAP_TYPE heapType
		);
		static void initializePlatformTable();
	};
#pragma endregion

#pragma region HeapFlags
    enum class EHeapFlag : uint32
    {
        None = 0,
        Shared = 1 << 0,
        CrossAdapter = 1 << 1,
        Protected = 1 << 2,
        NotResident = 1 << 3,

        Max = NotResident
    };

    struct HeapFlagInfoBase
    {
        FlyweightStringA _name;
        D3D12_HEAP_FLAGS _flag = D3D12_HEAP_FLAG_NONE;
    };

    struct D3D12HeapFlagInfo : public HeapFlagInfoBase, public EnumFlagTable(EHeapFlag, HeapFlagInfoBase, EHeapFlag::Max, D3D12HeapFlagInfo)
    {
        D3D12HeapFlagInfo() = default;
        D3D12HeapFlagInfo(
            const char* name,
            D3D12_HEAP_FLAGS flag);

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

	struct D3D12ResourceDimensionInfo : public ResourceDimensionInfoBase, public EnumTable(EResourceDimension, ResourceDimensionInfoBase, EResourceDimension::Count, D3D12ResourceDimensionInfo)
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
    enum class EResourceFlag : uint32
    {
        None                    = 0,
        DenyShaderResource      = 1 << 0,
        RenderTarget            = 1 << 1,
        DepthStencil            = 1 << 2,
        UnorderedAccess         = 1 << 3,
        SimultaneousAccess      = 1 << 4,
        CrossAdapter            = 1 << 5,
        AccelerationStructure   = 1 << 6,

        Max = AccelerationStructure
    };

	struct ResourceFlagInfoBase
    {
        FlyweightStringA    _name;
        D3D12_RESOURCE_FLAGS _flag = D3D12_RESOURCE_FLAG_NONE;
    };

    struct D3D12ResourceFlagInfo : public ResourceFlagInfoBase, public EnumFlagTable(EResourceFlag, ResourceFlagInfoBase, EResourceFlag::Max, D3D12ResourceFlagInfo)
    {
        D3D12ResourceFlagInfo() = default;
        D3D12ResourceFlagInfo(
            const char* name
            , D3D12_RESOURCE_FLAGS flag
        );
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region ResourceState
	enum class EResourceState : uint32
	{
		Common                            = 0,
		VertexAndConstantBuffer           = 1 << 0,
		IndexBuffer                       = 1 << 1,
		RenderTarget                      = 1 << 2,
		UnorderedAccess                   = 1 << 3,
		DepthWrite                        = 1 << 4,
		DepthRead                         = 1 << 5,
		NonPixelShaderResource            = 1 << 6,
		PixelShaderResource               = 1 << 7,
		StreamOut                         = 1 << 8,
		IndirectArgument                  = 1 << 9,
		CopyDest                          = 1 << 10,
		CopySource                        = 1 << 11,
		ResolveDest                       = 1 << 12,
		ResolveSource                     = 1 << 13,
		RaytracingAccelerationStructure   = 1 << 14,

        Max = RaytracingAccelerationStructure
	};

    struct ResourceStateInfoBase
	{
		FlyweightStringA    _name;
		D3D12_RESOURCE_STATES _state = D3D12_RESOURCE_STATE_COMMON;
	};
	
    struct D3D12ResourceStateInfo : public ResourceStateInfoBase, public EnumFlagTable(EResourceState, ResourceStateInfoBase, EResourceState::Max, D3D12ResourceStateInfo)
	{
		D3D12ResourceStateInfo() = default;
		D3D12ResourceStateInfo(
			const char* name
			, D3D12_RESOURCE_STATES state
		);
		static void initializePlatformTable();
	};
#pragma endregion

#pragma region DescriptorType
    enum class EDescriptorType : uint8
    {
        UniformBuffer,
        StorageBufferReadOnly,
        StorageBufferReadWrite,
        SampledImage,
        StorageImage,
        Sampler,
        Count
    };

	struct DescriptorTypeInfoBase
	{
		FlyweightStringA _name;
        D3D12_DESCRIPTOR_RANGE_TYPE _type = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	};

    struct D3D12DescriptorTypeInfo : public DescriptorTypeInfoBase, public EnumTable(EDescriptorType, DescriptorTypeInfoBase, EDescriptorType::Count, D3D12DescriptorTypeInfo)
    {
        D3D12DescriptorTypeInfo() = default;
        D3D12DescriptorTypeInfo(
            const char* name
            , D3D12_DESCRIPTOR_RANGE_TYPE type
        );
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region ShaderStage
	enum class EShaderStage : uint8
	{
		Compute,
		Vertex,
		Pixel,
		Hull,
		Domain,
		Geometry,
		Amplification,
		Mesh,
        All = Compute,
        Count
	};
	struct ShaderStageInfoBase
	{
		FlyweightStringA _name;
		D3D12_SHADER_VISIBILITY _visibility = D3D12_SHADER_VISIBILITY_ALL;
	};
	struct D3D12ShaderStageInfo : public ShaderStageInfoBase, public EnumTable(EShaderStage, ShaderStageInfoBase, EShaderStage::Count, D3D12ShaderStageInfo)
	{
		D3D12ShaderStageInfo() = default;
		D3D12ShaderStageInfo(
			const char* name
			, D3D12_SHADER_VISIBILITY visibility
		);
		static void initializePlatformTable();
	};
#pragma endregion

#pragma region Filter
	enum class EFilterType : uint8
    {
        MinMagMipPoint,
        MinMagMipLinear,
        Anisotropic,
        Count
    };

	struct FilterInfoBase
	{
		FlyweightStringA _name;
		D3D12_FILTER _filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	};

	struct D3D12FilterInfo : public FilterInfoBase, public EnumTable(EFilterType, FilterInfoBase, EFilterType::Count, D3D12FilterInfo)
	{
		D3D12FilterInfo() = default;
		D3D12FilterInfo(
			const char* name
			, D3D12_FILTER filter
		);
		static void initializePlatformTable();
	};
#pragma endregion

#pragma region SamplerAddressMode
	enum class ESamplerAddressMode : uint8
	{
		Wrap,
		Mirror,
		Clamp,
		Border,
		Count
	};
	struct SamplerAddressModeInfoBase
	{
		FlyweightStringA _name;
		D3D12_TEXTURE_ADDRESS_MODE _addressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	};
	struct D3D12SamplerAddressModeInfo : public SamplerAddressModeInfoBase, public EnumTable(ESamplerAddressMode, SamplerAddressModeInfoBase, ESamplerAddressMode::Count, D3D12SamplerAddressModeInfo)
	{
		D3D12SamplerAddressModeInfo() = default;
		D3D12SamplerAddressModeInfo(
			const char* name
			, D3D12_TEXTURE_ADDRESS_MODE addressMode
		);
		static void initializePlatformTable();
	};
#pragma endregion

#pragma region ComparisonFunction
	enum class EComparisonFunction : uint8
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always,
		Count
	};
	struct ComparisonFunctionInfoBase
	{
		FlyweightStringA _name;
		D3D12_COMPARISON_FUNC _comparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	};
	struct D3D12ComparisonFunctionInfo : public ComparisonFunctionInfoBase, public EnumTable(EComparisonFunction, ComparisonFunctionInfoBase, EComparisonFunction::Count, D3D12ComparisonFunctionInfo)
	{
		D3D12ComparisonFunctionInfo() = default;
		D3D12ComparisonFunctionInfo(
			const char* name
			, D3D12_COMPARISON_FUNC comparisonFunc
		);
		static void initializePlatformTable();
	};
}
