#pragma once
#include "RhiEnum.h"

namespace keyh
{
#pragma region ResourceFormat
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
            const char* name,
            bool canPresent,
            D3D12_COMMAND_LIST_TYPE type
        );

        static void initializePlatformTable();
    };
#pragma endregion

#pragma region HeapType
    struct HeapTypeInfoBase
    {
        FlyweightStringA    _name;
        D3D12_HEAP_TYPE     _heapType = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
    };

    struct D3D12HeapTypeInfo : public HeapTypeInfoBase, public EnumTable(EHeapType, HeapTypeInfoBase, EHeapType::Count, D3D12HeapTypeInfo)
    {
        D3D12HeapTypeInfo() = default;
        D3D12HeapTypeInfo(
            const char* name,
            D3D12_HEAP_TYPE heapType
        );
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region HeapFlags
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
    struct ResourceDimensionInfoBase
    {
        FlyweightStringA         _name;
        D3D12_RESOURCE_DIMENSION _dimension = D3D12_RESOURCE_DIMENSION_UNKNOWN;
    };

    struct D3D12ResourceDimensionInfo : public ResourceDimensionInfoBase, public EnumTable(EResourceDimension, ResourceDimensionInfoBase, EResourceDimension::Count, D3D12ResourceDimensionInfo)
    {
        D3D12ResourceDimensionInfo() = default;
        D3D12ResourceDimensionInfo(
            const char* name,
            D3D12_RESOURCE_DIMENSION dimension
        );
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region ResourceFlag
    struct ResourceFlagInfoBase
    {
        FlyweightStringA     _name;
        D3D12_RESOURCE_FLAGS _flag = D3D12_RESOURCE_FLAG_NONE;
    };

    struct D3D12ResourceFlagInfo : public ResourceFlagInfoBase, public EnumFlagTable(EResourceFlag, ResourceFlagInfoBase, EResourceFlag::Max, D3D12ResourceFlagInfo)
    {
        D3D12ResourceFlagInfo() = default;
        D3D12ResourceFlagInfo(
            const char* name,
            D3D12_RESOURCE_FLAGS flag
        );
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region ResourceState
    struct ResourceStateInfoBase
    {
        FlyweightStringA      _name;
        D3D12_RESOURCE_STATES _state = D3D12_RESOURCE_STATE_COMMON;
    };

    struct D3D12ResourceStateInfo : public ResourceStateInfoBase, public EnumFlagTable(EResourceState, ResourceStateInfoBase, EResourceState::Max, D3D12ResourceStateInfo)
    {
        D3D12ResourceStateInfo() = default;
        D3D12ResourceStateInfo(
            const char* name,
            D3D12_RESOURCE_STATES state
        );
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region DescriptorType
    struct DescriptorTypeInfoBase
    {
        FlyweightStringA            _name;
        D3D12_DESCRIPTOR_RANGE_TYPE _type = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    };

    struct D3D12DescriptorTypeInfo : public DescriptorTypeInfoBase, public EnumTable(EDescriptorType, DescriptorTypeInfoBase, EDescriptorType::Count, D3D12DescriptorTypeInfo)
    {
        D3D12DescriptorTypeInfo() = default;
        D3D12DescriptorTypeInfo(
            const char* name,
            D3D12_DESCRIPTOR_RANGE_TYPE type
        );
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region ShaderStage
    struct ShaderStageInfoBase
    {
        FlyweightStringA       _name;
        D3D12_SHADER_VISIBILITY _visibility = D3D12_SHADER_VISIBILITY_ALL;
    };

    struct D3D12ShaderStageInfo : public ShaderStageInfoBase, public EnumTable(EShaderStage, ShaderStageInfoBase, EShaderStage::Count, D3D12ShaderStageInfo)
    {
        D3D12ShaderStageInfo() = default;
        D3D12ShaderStageInfo(
            const char* name,
            D3D12_SHADER_VISIBILITY visibility
        );
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region Filter
    struct FilterInfoBase
    {
        FlyweightStringA _name;
        D3D12_FILTER     _filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    };

    struct D3D12FilterInfo : public FilterInfoBase, public EnumTable(EFilterType, FilterInfoBase, EFilterType::Count, D3D12FilterInfo)
    {
        D3D12FilterInfo() = default;
        D3D12FilterInfo(
            const char* name,
            D3D12_FILTER filter
        );
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region SamplerAddressMode
    struct SamplerAddressModeInfoBase
    {
        FlyweightStringA          _name;
        D3D12_TEXTURE_ADDRESS_MODE _addressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    };

    struct D3D12SamplerAddressModeInfo : public SamplerAddressModeInfoBase, public EnumTable(ESamplerAddressMode, SamplerAddressModeInfoBase, ESamplerAddressMode::Count, D3D12SamplerAddressModeInfo)
    {
        D3D12SamplerAddressModeInfo() = default;
        D3D12SamplerAddressModeInfo(
            const char* name,
            D3D12_TEXTURE_ADDRESS_MODE addressMode
        );
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region ComparisonFunction
    struct ComparisonFunctionInfoBase
    {
        FlyweightStringA      _name;
        D3D12_COMPARISON_FUNC _comparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    };

    struct D3D12ComparisonFunctionInfo : public ComparisonFunctionInfoBase, public EnumTable(EComparisonFunction, ComparisonFunctionInfoBase, EComparisonFunction::Count, D3D12ComparisonFunctionInfo)
    {
        D3D12ComparisonFunctionInfo() = default;
        D3D12ComparisonFunctionInfo(
            const char* name,
            D3D12_COMPARISON_FUNC comparisonFunc
        );
        static void initializePlatformTable();
    };
#pragma endregion
}
