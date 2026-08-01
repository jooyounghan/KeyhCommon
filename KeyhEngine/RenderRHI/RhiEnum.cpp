#include "RenderRhiPch.h"
#include "RhiEnum.h"

#define UINT(x) static_cast<uint32>(x)
namespace keyh
{
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
        : ResourceFormatInfoBase{ name, bytesPerPixel, blockSize, isDepthFormat, isCompressed }
        , _format(format)
        , _srgbFormat(srgbFormat)
    {}

    void D3D12ResourceFormatInfo::initializePlatformTable()
    {
        _table[UINT(EResourceFormat::Unknown)] = D3D12ResourceFormatInfo{ "UNKNOWN", 0, 0, false, false, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN };
        _table[UINT(EResourceFormat::R32G32B32A32_Float)] = D3D12ResourceFormatInfo{ "R32G32B32A32_FLOAT", 16, 1, false, false, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT };
        _table[UINT(EResourceFormat::B8G8R8A8_UNorm)] = D3D12ResourceFormatInfo{ "B8G8R8A8_UNORM", 4, 1, false, false, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB };
        _table[UINT(EResourceFormat::R8G8B8A8_UNorm)] = D3D12ResourceFormatInfo{ "R8G8B8A8_UNORM", 4, 1, false, false, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB };
        _table[UINT(EResourceFormat::R32_Float)] = D3D12ResourceFormatInfo{ "R32_FLOAT", 4, 1, false, false, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_R32_FLOAT };
    }
#pragma endregion

#pragma region CommandQueue
    D3D12CommandQueueInfo::D3D12CommandQueueInfo(
        const char* name
        , bool canPresent
        , D3D12_COMMAND_LIST_TYPE type
    )
        : CommandQueueInfoBase{ name, canPresent }
        , _type(type)
    {
    
    }

    void D3D12CommandQueueInfo::initializePlatformTable()
    {
        _table[UINT(ECommandQueueType::Direct)] = D3D12CommandQueueInfo{ "Direct", true, D3D12_COMMAND_LIST_TYPE_DIRECT };
        _table[UINT(ECommandQueueType::Compute)] = D3D12CommandQueueInfo{ "Compute", false, D3D12_COMMAND_LIST_TYPE_COMPUTE };
        _table[UINT(ECommandQueueType::Copy)] = D3D12CommandQueueInfo{ "Copy", false, D3D12_COMMAND_LIST_TYPE_COPY };
    }
#pragma endregion
}

#undef UINT