#include "RenderRhiPch.h"
#include "RhiEnum.h"

#define UINT(x) static_cast<uint32>(x)
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
        getTable()[UINT(EResourceFormat::Unknown)] = D3D12ResourceFormatInfo{ "UNKNOWN", 0, 0, false, false, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN };
        getTable()[UINT(EResourceFormat::R32G32B32A32_Float)] = D3D12ResourceFormatInfo{ "R32G32B32A32_FLOAT", 16, 1, false, false, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT };
        getTable()[UINT(EResourceFormat::B8G8R8A8_UNorm)] = D3D12ResourceFormatInfo{ "B8G8R8A8_UNORM", 4, 1, false, false, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB };
        getTable()[UINT(EResourceFormat::R8G8B8A8_UNorm)] = D3D12ResourceFormatInfo{ "R8G8B8A8_UNORM", 4, 1, false, false, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB };
        getTable()[UINT(EResourceFormat::R32_Float)] = D3D12ResourceFormatInfo{ "R32_FLOAT", 4, 1, false, false, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_R32_FLOAT };
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
        getTable()[UINT(ECommandQueueType::Direct)] = D3D12CommandQueueInfo{ "Direct", true, D3D12_COMMAND_LIST_TYPE_DIRECT };
        getTable()[UINT(ECommandQueueType::Compute)] = D3D12CommandQueueInfo{ "Compute", false, D3D12_COMMAND_LIST_TYPE_COMPUTE };
        getTable()[UINT(ECommandQueueType::Copy)] = D3D12CommandQueueInfo{ "Copy", false, D3D12_COMMAND_LIST_TYPE_COPY };
    }
#pragma endregion

#pragma region HeapType
    D3D12HeapTypeInfo::D3D12HeapTypeInfo(const char* name, D3D12_HEAP_TYPE heapType)
		: HeapTypeInfoBase{ name, heapType }
    {}

    void D3D12HeapTypeInfo::initializePlatformTable()
    {
        getTable()[UINT(EHeapType::Default)] = D3D12HeapTypeInfo{ "Default", D3D12_HEAP_TYPE_DEFAULT };
		getTable()[UINT(EHeapType::Upload)] = D3D12HeapTypeInfo{ "Upload", D3D12_HEAP_TYPE_UPLOAD };
		getTable()[UINT(EHeapType::Readback)] = D3D12HeapTypeInfo{ "Readback", D3D12_HEAP_TYPE_READBACK };
    }
#pragma endregion

#pragma region ResourceDimension
    D3D12ResourceDimensionInfo::D3D12ResourceDimensionInfo(const char* name, D3D12_RESOURCE_DIMENSION dimension)
        : ResourceDimensionInfoBase{ name, dimension }
    {}

    void D3D12ResourceDimensionInfo::initializePlatformTable()
    {
        getTable()[UINT(EResourceDimension::Buffer)] = D3D12ResourceDimensionInfo{ "Buffer", D3D12_RESOURCE_DIMENSION_BUFFER };
        getTable()[UINT(EResourceDimension::Texture1D)] = D3D12ResourceDimensionInfo{ "Texture1D", D3D12_RESOURCE_DIMENSION_TEXTURE1D };
        getTable()[UINT(EResourceDimension::Texture2D)] = D3D12ResourceDimensionInfo{ "Texture2D", D3D12_RESOURCE_DIMENSION_TEXTURE2D };
		getTable()[UINT(EResourceDimension::Texture3D)] = D3D12ResourceDimensionInfo{ "Texture3D", D3D12_RESOURCE_DIMENSION_TEXTURE3D };
    }
#pragma endregion


#pragma region ResourceFlag
    D3D12ResourceFlagInfo::D3D12ResourceFlagInfo(const char* name, D3D12_RESOURCE_FLAGS flags)
        : ResourceFlagInfoBase{ name, flags }
    {}

    void D3D12ResourceFlagInfo::initializePlatformTable()
    {
		getTable()[UINT(EResourceFlag::None)] = D3D12ResourceFlagInfo{ "None", D3D12_RESOURCE_FLAG_NONE };
        getTable()[UINT(EResourceFlag::DenyShaderResource)] = D3D12ResourceFlagInfo{ "DenyShaderResource", D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE };
		getTable()[UINT(EResourceFlag::RenderTarget)] = D3D12ResourceFlagInfo{ "RenderTarget", D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET };
		getTable()[UINT(EResourceFlag::DepthStencil)] = D3D12ResourceFlagInfo{ "DepthStencil", D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL };
		getTable()[UINT(EResourceFlag::UnorderedAccess)] = D3D12ResourceFlagInfo{ "UnorderedAccess", D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS };
		getTable()[UINT(EResourceFlag::SimultaneousAccess)] = D3D12ResourceFlagInfo{ "SimultaneousAccess", D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS }; 
		getTable()[UINT(EResourceFlag::CrossAdapter)] = D3D12ResourceFlagInfo{ "CrossAdapter", D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER }; 
    }
#pragma endregion
    D3D12ResourceStateInfo::D3D12ResourceStateInfo(const char* name, D3D12_RESOURCE_STATES state)
        : ResourceStateInfoBase{ name, state }
    {}

    void D3D12ResourceStateInfo::initializePlatformTable()
    {
    
    }
}

#undef UINT