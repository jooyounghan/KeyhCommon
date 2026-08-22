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
        inline static const Derived& getInfo(EnumType type) { return getTable()[static_cast<uint32>(type)]; }

    protected:
        static void registerEntry(EnumType type, Derived info)
        {
            getTable()[static_cast<uint32>(type)] = keyh::move(info);
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
#pragma endregion

#pragma region CommandQueueType
	enum class ECommandQueueType : uint8
	{
		Direct,
		Compute,
		Copy,
		Count
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
#pragma endregion

#pragma region Filter
	enum class EFilterType : uint8
    {
        MinMagMipPoint,
        MinMagMipLinear,
        Anisotropic,
        Count
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
#pragma endregion

#pragma region FillMode
	enum class EFillMode : uint8
	{
		Wireframe,
		Solid,
		Count
	};
#pragma endregion

#pragma region CullMode
	enum class ECullMode : uint8
	{
		None,
		Front,
		Back,
		Count
	};
#pragma endregion

#pragma region BlendFactor
	enum class EBlendFactor : uint8
	{
		Zero,
		One,
		SrcColor,
		InvSrcColor,
		SrcAlpha,
		InvSrcAlpha,
		DstAlpha,
		InvDstAlpha,
		DstColor,
		InvDstColor,
		SrcAlphaSat,
		ConstantColor,
		InvConstantColor,
		Src1Color,
		InvSrc1Color,
		Src1Alpha,
		InvSrc1Alpha,
		Count
	};
#pragma endregion

#pragma region BlendOp
	enum class EBlendOp : uint8
	{
		Add,
		Subtract,
		RevSubtract,
		Min,
		Max,
		Count
	};
#pragma endregion

#pragma region LogicOp
	enum class ELogicOp : uint8
	{
		Clear,
		Set,
		Copy,
		CopyInverted,
		Noop,
		Invert,
		And,
		Nand,
		Or,
		Nor,
		Xor,
		Equiv,
		AndReverse,
		AndInverted,
		OrReverse,
		OrInverted,
		Count
	};
#pragma endregion

#pragma region ColorWriteMask
	enum class EColorWriteMask : uint8
	{
		None  = 0,
		Red   = 1 << 0,
		Green = 1 << 1,
		Blue  = 1 << 2,
		Alpha = 1 << 3,
		All   = Red | Green | Blue | Alpha,
		Max   = All
	};
#pragma endregion

#pragma region StencilOp
	enum class EStencilOp : uint8
	{
		Keep,
		Zero,
		Replace,
		IncrSat,
		DecrSat,
		Invert,
		Incr,
		Decr,
		Count
	};
#pragma endregion

#pragma region PrimitiveTopologyType
	enum class EPrimitiveTopologyType : uint8
	{
		Undefined,
		Point,
		Line,
		Triangle,
		Patch,
		Count
	};
#pragma endregion

#pragma region RhiViewport
	struct RhiViewport
	{
		float x        = 0.f;
		float y        = 0.f;
		float width    = 0.f;
		float height   = 0.f;
		float minDepth = 0.f;
		float maxDepth = 1.f;
	};
#pragma endregion

#pragma region RhiRect
	struct RhiRect
	{
		int32 left   = 0;
		int32 top    = 0;
		int32 right  = 0;
		int32 bottom = 0;
	};
#pragma endregion

#pragma region RhiCpuDescriptorHandle
	struct RhiCpuDescriptorHandle
	{
		size_t ptr = 0;
	};
#pragma endregion

#pragma region RhiVertexBufferView
	struct RhiVertexBufferView
	{
		size_t _bufferLocation = 0;
		uint32 _sizeInBytes    = 0;
		uint32 _strideInBytes  = 0;
	};
#pragma endregion

}
