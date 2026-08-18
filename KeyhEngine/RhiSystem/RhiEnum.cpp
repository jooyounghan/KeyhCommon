#include "RhiSystemPch.h"
#include "RhiEnum.h"

namespace keyh
{
    template<typename EnumType, typename InfoType, uint32 Count, typename Derived>
    InfoList<Derived, Count> IEnumInfoTable<EnumType, InfoType, Count, Derived, true>::getInfoList(EnumType combinedFlags)
    {
        InfoList<Derived, Count> list;
        uint32 mask = static_cast<uint32>(combinedFlags);

        for (uint32 i = 0; i < Count; ++i)
        {
            uint32 bitValue = (1 << i);
            if ((mask & bitValue) != 0)
            {
                list._items[list._count] = &getTable()[i];
                list._count++;
            }
        }

        return list;
    }

#pragma region ResourceFormat
	D3D12ResourceFormatInfo::D3D12ResourceFormatInfo(
        const char* name
        , unsigned int bytesPerPixel
        , unsigned int blockSize
        , bool isDepthFormat
        , bool isCompressed
        , DXGI_FORMAT format
        , DXGI_FORMAT srgbFormat
    )
        : ResourceFormatInfoBase{ name, bytesPerPixel, blockSize, isDepthFormat, isCompressed, format, srgbFormat }
    {}

    void D3D12ResourceFormatInfo::initializePlatformTable()
    {
        registerEntry(EResourceFormat::Unknown,           { "UNKNOWN",           0,  0, false, false, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN,           DXGI_FORMAT::DXGI_FORMAT_UNKNOWN           });
        registerEntry(EResourceFormat::R32G32B32A32_Float,{ "R32G32B32A32_FLOAT",16, 1, false, false, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT });
        registerEntry(EResourceFormat::B8G8R8A8_UNorm,    { "B8G8R8A8_UNORM",    4,  1, false, false, DXGI_FORMAT_B8G8R8A8_UNORM,                  DXGI_FORMAT_B8G8R8A8_UNORM_SRGB            });
        registerEntry(EResourceFormat::R8G8B8A8_UNorm,    { "R8G8B8A8_UNORM",    4,  1, false, false, DXGI_FORMAT_R8G8B8A8_UNORM,                  DXGI_FORMAT_R8G8B8A8_UNORM_SRGB            });
        registerEntry(EResourceFormat::R32_Float,         { "R32_FLOAT",         4,  1, false, false, DXGI_FORMAT_R32_FLOAT,                        DXGI_FORMAT_R32_FLOAT                      });
    }
#pragma endregion

#pragma region CommandQueue
    D3D12CommandQueueInfo::D3D12CommandQueueInfo(
        const char* name
        , bool canPresent
        , D3D12_COMMAND_LIST_TYPE type
    )
        : CommandQueueInfoBase{ name, canPresent, type }
    {
    
    }

    void D3D12CommandQueueInfo::initializePlatformTable()
    {
        registerEntry(ECommandQueueType::Direct,  { "Direct",  true,  D3D12_COMMAND_LIST_TYPE_DIRECT  });
        registerEntry(ECommandQueueType::Compute, { "Compute", false, D3D12_COMMAND_LIST_TYPE_COMPUTE });
        registerEntry(ECommandQueueType::Copy,    { "Copy",    false, D3D12_COMMAND_LIST_TYPE_COPY    });
    }
#pragma endregion

#pragma region HeapType
    D3D12HeapTypeInfo::D3D12HeapTypeInfo(const char* name, D3D12_HEAP_TYPE heapType)
		: HeapTypeInfoBase{ name, heapType }
    {}

    void D3D12HeapTypeInfo::initializePlatformTable()
    {
        registerEntry(EHeapType::Default,  { "Default",  D3D12_HEAP_TYPE_DEFAULT  });
        registerEntry(EHeapType::Upload,   { "Upload",   D3D12_HEAP_TYPE_UPLOAD   });
        registerEntry(EHeapType::Readback, { "Readback", D3D12_HEAP_TYPE_READBACK });
    }
#pragma endregion

#pragma region HeapFlag
	D3D12HeapFlagInfo::D3D12HeapFlagInfo(const char* name, D3D12_HEAP_FLAGS flag)
		: HeapFlagInfoBase{ name, flag }
	{}

	void D3D12HeapFlagInfo::initializePlatformTable()
	{
		registerEntry(EHeapFlag::None            , { "None", D3D12_HEAP_FLAG_NONE });
		registerEntry(EHeapFlag::Shared          , { "Shared", D3D12_HEAP_FLAG_SHARED });
		registerEntry(EHeapFlag::CrossAdapter    , { "CrossAdapter", D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER });
		registerEntry(EHeapFlag::Protected       , { "Protected", D3D12_HEAP_FLAG_HARDWARE_PROTECTED });
		registerEntry(EHeapFlag::NotResident     , { "NotResident", D3D12_HEAP_FLAG_CREATE_NOT_RESIDENT });
	}
#pragma endregion

#pragma region ResourceDimension
    D3D12ResourceDimensionInfo::D3D12ResourceDimensionInfo(const char* name, D3D12_RESOURCE_DIMENSION dimension)
        : ResourceDimensionInfoBase{ name, dimension }
    {}

    void D3D12ResourceDimensionInfo::initializePlatformTable()
    {
        registerEntry(EResourceDimension::Buffer,    { "Buffer",    D3D12_RESOURCE_DIMENSION_BUFFER    });
        registerEntry(EResourceDimension::Texture1D, { "Texture1D", D3D12_RESOURCE_DIMENSION_TEXTURE1D });
        registerEntry(EResourceDimension::Texture2D, { "Texture2D", D3D12_RESOURCE_DIMENSION_TEXTURE2D });
        registerEntry(EResourceDimension::Texture3D, { "Texture3D", D3D12_RESOURCE_DIMENSION_TEXTURE3D });
    }
#pragma endregion


#pragma region ResourceFlag
    D3D12ResourceFlagInfo::D3D12ResourceFlagInfo(const char* name, D3D12_RESOURCE_FLAGS flags)
        : ResourceFlagInfoBase{ name, flags }
    {}

    void D3D12ResourceFlagInfo::initializePlatformTable()
    {
        registerEntry(EResourceFlag::None                   , { "None",               D3D12_RESOURCE_FLAG_NONE });
        registerEntry(EResourceFlag::DenyShaderResource     , { "DenyShaderResource", D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE });
        registerEntry(EResourceFlag::RenderTarget           , { "RenderTarget",       D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET });
        registerEntry(EResourceFlag::DepthStencil           , { "DepthStencil",       D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL });
        registerEntry(EResourceFlag::UnorderedAccess        , { "UnorderedAccess",    D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS });
        registerEntry(EResourceFlag::SimultaneousAccess     , { "SimultaneousAccess", D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS });
        registerEntry(EResourceFlag::CrossAdapter           , { "CrossAdapter",       D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER });
        registerEntry(EResourceFlag::AccelerationStructure  , { "AccelerationStructure", D3D12_RESOURCE_FLAG_RAYTRACING_ACCELERATION_STRUCTURE });
    }
#pragma endregion

#pragma region ResourceState
    D3D12ResourceStateInfo::D3D12ResourceStateInfo(const char* name, D3D12_RESOURCE_STATES state)
        : ResourceStateInfoBase{ name, state }
    {}

    void D3D12ResourceStateInfo::initializePlatformTable()
    {
		registerEntry(EResourceState::Common                            , { "Common", D3D12_RESOURCE_STATE_COMMON });
		registerEntry(EResourceState::VertexAndConstantBuffer           , { "VertexAndConstantBuffer", D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER });
		registerEntry(EResourceState::IndexBuffer                       , { "IndexBuffer", D3D12_RESOURCE_STATE_INDEX_BUFFER });
		registerEntry(EResourceState::RenderTarget                      , { "RenderTarget", D3D12_RESOURCE_STATE_RENDER_TARGET });
		registerEntry(EResourceState::UnorderedAccess                   , { "UnorderedAccess", D3D12_RESOURCE_STATE_UNORDERED_ACCESS });
		registerEntry(EResourceState::DepthWrite                        , { "DepthWrite", D3D12_RESOURCE_STATE_DEPTH_WRITE });
		registerEntry(EResourceState::DepthRead                         , { "DepthRead", D3D12_RESOURCE_STATE_DEPTH_READ });
		registerEntry(EResourceState::NonPixelShaderResource            , { "NonPixelShaderResource", D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE });
		registerEntry(EResourceState::PixelShaderResource               , { "PixelShaderResource", D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE });
		registerEntry(EResourceState::StreamOut                         , { "StreamOut", D3D12_RESOURCE_STATE_STREAM_OUT });
		registerEntry(EResourceState::IndirectArgument                  , { "IndirectArgument", D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT });
		registerEntry(EResourceState::CopyDest                          , { "CopyDest", D3D12_RESOURCE_STATE_COPY_DEST });
		registerEntry(EResourceState::CopySource                        , { "CopySource", D3D12_RESOURCE_STATE_COPY_SOURCE });
		registerEntry(EResourceState::ResolveDest                       , { "ResolveDest", D3D12_RESOURCE_STATE_RESOLVE_DEST });
		registerEntry(EResourceState::ResolveSource                     , { "ResolveSource", D3D12_RESOURCE_STATE_RESOLVE_SOURCE });
		registerEntry(EResourceState::RaytracingAccelerationStructure   , { "RaytracingAccelerationStructure", D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE });
    }
#pragma endregion

#pragma region DescriptorType
    D3D12DescriptorTypeInfo::D3D12DescriptorTypeInfo(const char* name, D3D12_DESCRIPTOR_RANGE_TYPE type)
		: DescriptorTypeInfoBase{ name, type }
    {}

    void D3D12DescriptorTypeInfo::initializePlatformTable()
    {
		registerEntry(EDescriptorType::UniformBuffer             , { "UniformBuffer", D3D12_DESCRIPTOR_RANGE_TYPE_CBV });
		registerEntry(EDescriptorType::StorageBufferReadOnly     , { "StorageBufferReadOnly", D3D12_DESCRIPTOR_RANGE_TYPE_SRV });
		registerEntry(EDescriptorType::StorageBufferReadWrite    , { "StorageBufferReadWrite", D3D12_DESCRIPTOR_RANGE_TYPE_UAV });
		registerEntry(EDescriptorType::SampledImage              , { "SampledImage", D3D12_DESCRIPTOR_RANGE_TYPE_SRV });
		registerEntry(EDescriptorType::StorageImage              , { "StorageImage", D3D12_DESCRIPTOR_RANGE_TYPE_UAV });
		registerEntry(EDescriptorType::Sampler                   , { "Sampler", D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER });
    }
#pragma endregion

#pragma region ShaderStage
    D3D12ShaderStageInfo::D3D12ShaderStageInfo(const char* name, D3D12_SHADER_VISIBILITY visibility)
		: ShaderStageInfoBase{ name, visibility }
    {}

    void D3D12ShaderStageInfo::initializePlatformTable()
    {
        registerEntry(EShaderStage::All             , { "All", D3D12_SHADER_VISIBILITY_ALL } );
		registerEntry(EShaderStage::Vertex          , { "Vertex",  D3D12_SHADER_VISIBILITY_VERTEX });
		registerEntry(EShaderStage::Pixel           , { "Pixel", D3D12_SHADER_VISIBILITY_PIXEL });
		registerEntry(EShaderStage::Hull            , { "Hull", D3D12_SHADER_VISIBILITY_HULL });
		registerEntry(EShaderStage::Domain          , { "Domain", D3D12_SHADER_VISIBILITY_DOMAIN });
		registerEntry(EShaderStage::Geometry        , { "Geometry", D3D12_SHADER_VISIBILITY_GEOMETRY });
        registerEntry(EShaderStage::Amplification   , { "Amplification", D3D12_SHADER_VISIBILITY_AMPLIFICATION });
        registerEntry(EShaderStage::Mesh            , { "Mesh", D3D12_SHADER_VISIBILITY_MESH });
    }
#pragma endregion

#pragma region Filter
    D3D12FilterInfo::D3D12FilterInfo(const char* name, D3D12_FILTER filter)
		: FilterInfoBase{ name, filter }
    {}

	void D3D12FilterInfo::initializePlatformTable()
	{
		registerEntry(EFilterType::MinMagMipPoint   , { "MinMagMipPoint", D3D12_FILTER_MIN_MAG_MIP_POINT });
		registerEntry(EFilterType::MinMagMipLinear  , { "MinMagMipLinear", D3D12_FILTER_MIN_MAG_MIP_LINEAR });
		registerEntry(EFilterType::Anisotropic      , { "Anisotropic", D3D12_FILTER_ANISOTROPIC });
	}
#pragma endregion

#pragma region SamplerAddressMode
    D3D12SamplerAddressModeInfo::D3D12SamplerAddressModeInfo(const char* name, D3D12_TEXTURE_ADDRESS_MODE addressMode)
		: SamplerAddressModeInfoBase{ name, addressMode }
    {}

    void D3D12SamplerAddressModeInfo::initializePlatformTable()
    {
		registerEntry(ESamplerAddressMode::Wrap     , { "Wrap", D3D12_TEXTURE_ADDRESS_MODE_WRAP });
		registerEntry(ESamplerAddressMode::Mirror   , { "Mirror", D3D12_TEXTURE_ADDRESS_MODE_MIRROR });
		registerEntry(ESamplerAddressMode::Clamp    , { "Clamp", D3D12_TEXTURE_ADDRESS_MODE_CLAMP });
		registerEntry(ESamplerAddressMode::Border   , { "Border", D3D12_TEXTURE_ADDRESS_MODE_BORDER });
    }
#pragma endregion

#pragma region ComparisonFunction
	D3D12ComparisonFunctionInfo::D3D12ComparisonFunctionInfo(const char* name, D3D12_COMPARISON_FUNC comparisonFunc)
		: ComparisonFunctionInfoBase{ name, comparisonFunc }
	{}

	void D3D12ComparisonFunctionInfo::initializePlatformTable()
	{
		registerEntry(EComparisonFunction::Never        , { "Never", D3D12_COMPARISON_FUNC_NEVER });
		registerEntry(EComparisonFunction::Less         , { "Less", D3D12_COMPARISON_FUNC_LESS });
		registerEntry(EComparisonFunction::Equal        , { "Equal", D3D12_COMPARISON_FUNC_EQUAL });
		registerEntry(EComparisonFunction::LessEqual    , { "LessEqual", D3D12_COMPARISON_FUNC_LESS_EQUAL });
		registerEntry(EComparisonFunction::Greater      , { "Greater", D3D12_COMPARISON_FUNC_GREATER });
		registerEntry(EComparisonFunction::NotEqual     , { "NotEqual", D3D12_COMPARISON_FUNC_NOT_EQUAL });
		registerEntry(EComparisonFunction::GreaterEqual , { "GreaterEqual", D3D12_COMPARISON_FUNC_GREATER_EQUAL });
		registerEntry(EComparisonFunction::Always       , { "Always", D3D12_COMPARISON_FUNC_ALWAYS });
	}
#pragma endregion
}