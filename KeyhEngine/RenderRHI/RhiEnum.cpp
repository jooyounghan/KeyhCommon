#include "RenderRhiPch.h"
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
    RhiDescriptorTypeInfo::RhiDescriptorTypeInfo(const char* name, D3D12_DESCRIPTOR_RANGE_TYPE type)
		: RhiDescriptorTypeInfoBase{ name, type }
    {}

    void RhiDescriptorTypeInfo::initializePlatformTable()
    {
		registerEntry(ERhiDescriptorType::UniformBuffer             , { "UniformBuffer", D3D12_DESCRIPTOR_RANGE_TYPE_CBV });
		registerEntry(ERhiDescriptorType::StorageBufferReadOnly     , { "StorageBufferReadOnly", D3D12_DESCRIPTOR_RANGE_TYPE_SRV });
		registerEntry(ERhiDescriptorType::StorageBufferReadWrite    , { "StorageBufferReadWrite", D3D12_DESCRIPTOR_RANGE_TYPE_UAV });
		registerEntry(ERhiDescriptorType::SampledImage              , { "SampledImage", D3D12_DESCRIPTOR_RANGE_TYPE_SRV });
		registerEntry(ERhiDescriptorType::StorageImage              , { "StorageImage", D3D12_DESCRIPTOR_RANGE_TYPE_UAV });
		registerEntry(ERhiDescriptorType::Sampler                   , { "Sampler", D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER });
    }
#pragma endregion
}