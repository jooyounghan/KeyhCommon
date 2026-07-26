#include "RenderRhiPch.h"
#include "RhiEnum.h"

namespace keyh
{
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
        _table[static_cast<uint32>(EResourceFormat::Unknown)] = D3D12ResourceFormatInfo{ "UNKNOWN", 0, 0, false, false, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN };
        _table[static_cast<uint32>(EResourceFormat::R32G32B32A32_Float)] = D3D12ResourceFormatInfo{ "R32G32B32A32_FLOAT", 16, 1, false, false, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT };
        _table[static_cast<uint32>(EResourceFormat::B8G8R8A8_UNorm)] = D3D12ResourceFormatInfo{ "B8G8R8A8_UNORM", 4, 1, false, false, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB };
        _table[static_cast<uint32>(EResourceFormat::R8G8B8A8_UNorm)] = D3D12ResourceFormatInfo{ "R8G8B8A8_UNORM", 4, 1, false, false, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB };
        _table[static_cast<uint32>(EResourceFormat::R32_Float)] = D3D12ResourceFormatInfo{ "R32_FLOAT", 4, 1, false, false, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_R32_FLOAT };
    }
}