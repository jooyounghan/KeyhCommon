#pragma once
#include "RhiEnum.h"

namespace keyh
{
#pragma region ResourceFormat
    struct ResourceFormatInfoBase
    {
        FlyweightStringA    _name           = FlyweightStringA::Empty;
        unsigned int        _bytesPerPixel  = 0;
        unsigned int        _blockSize      = 0;
        bool                _isDepthFormat  = false;
        bool                _isCompressed   = false;
        DXGI_FORMAT         _format         = DXGI_FORMAT_UNKNOWN;
        DXGI_FORMAT         _srgbFormat     = DXGI_FORMAT_UNKNOWN;
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

#pragma region FillMode
    struct FillModeInfoBase
    {
        FlyweightStringA  _name;
        D3D12_FILL_MODE   _fillMode = D3D12_FILL_MODE_SOLID;
    };

    struct D3D12FillModeInfo : public FillModeInfoBase, public EnumTable(EFillMode, FillModeInfoBase, EFillMode::Count, D3D12FillModeInfo)
    {
        D3D12FillModeInfo() = default;
        D3D12FillModeInfo(const char* name, D3D12_FILL_MODE fillMode);
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region CullMode
    struct CullModeInfoBase
    {
        FlyweightStringA  _name;
        D3D12_CULL_MODE   _cullMode = D3D12_CULL_MODE_NONE;
    };

    struct D3D12CullModeInfo : public CullModeInfoBase, public EnumTable(ECullMode, CullModeInfoBase, ECullMode::Count, D3D12CullModeInfo)
    {
        D3D12CullModeInfo() = default;
        D3D12CullModeInfo(const char* name, D3D12_CULL_MODE cullMode);
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region BlendFactor
    struct BlendFactorInfoBase
    {
        FlyweightStringA _name;
        D3D12_BLEND      _blend = D3D12_BLEND_ZERO;
    };

    struct D3D12BlendFactorInfo : public BlendFactorInfoBase, public EnumTable(EBlendFactor, BlendFactorInfoBase, EBlendFactor::Count, D3D12BlendFactorInfo)
    {
        D3D12BlendFactorInfo() = default;
        D3D12BlendFactorInfo(const char* name, D3D12_BLEND blend);
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region BlendOp
    struct BlendOpInfoBase
    {
        FlyweightStringA _name;
        D3D12_BLEND_OP   _blendOp = D3D12_BLEND_OP_ADD;
    };

    struct D3D12BlendOpInfo : public BlendOpInfoBase, public EnumTable(EBlendOp, BlendOpInfoBase, EBlendOp::Count, D3D12BlendOpInfo)
    {
        D3D12BlendOpInfo() = default;
        D3D12BlendOpInfo(const char* name, D3D12_BLEND_OP blendOp);
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region LogicOp
    struct LogicOpInfoBase
    {
        FlyweightStringA _name;
        D3D12_LOGIC_OP   _logicOp = D3D12_LOGIC_OP_NOOP;
    };

    struct D3D12LogicOpInfo : public LogicOpInfoBase, public EnumTable(ELogicOp, LogicOpInfoBase, ELogicOp::Count, D3D12LogicOpInfo)
    {
        D3D12LogicOpInfo() = default;
        D3D12LogicOpInfo(const char* name, D3D12_LOGIC_OP logicOp);
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region StencilOp
    struct StencilOpInfoBase
    {
        FlyweightStringA _name;
        D3D12_STENCIL_OP _stencilOp = D3D12_STENCIL_OP_KEEP;
    };

    struct D3D12StencilOpInfo : public StencilOpInfoBase, public EnumTable(EStencilOp, StencilOpInfoBase, EStencilOp::Count, D3D12StencilOpInfo)
    {
        D3D12StencilOpInfo() = default;
        D3D12StencilOpInfo(const char* name, D3D12_STENCIL_OP stencilOp);
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region PrimitiveTopologyType
    struct PrimitiveTopologyTypeInfoBase
    {
        FlyweightStringA               _name;
        D3D12_PRIMITIVE_TOPOLOGY_TYPE  _topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
        D3D_PRIMITIVE_TOPOLOGY         _topology     = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
    };

    struct D3D12PrimitiveTopologyTypeInfo : public PrimitiveTopologyTypeInfoBase, public EnumTable(EPrimitiveTopologyType, PrimitiveTopologyTypeInfoBase, EPrimitiveTopologyType::Count, D3D12PrimitiveTopologyTypeInfo)
    {
        D3D12PrimitiveTopologyTypeInfo() = default;
        D3D12PrimitiveTopologyTypeInfo(const char* name, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, D3D_PRIMITIVE_TOPOLOGY topology);
        static void initializePlatformTable();
    };
#pragma endregion

#pragma region ResourceBarrierType
    struct ResourceBarrierTypeInfoBase
    {
        FlyweightStringA          _name;
        D3D12_RESOURCE_BARRIER_TYPE _barrierType = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    };

    struct D3D12ResourceBarrierTypeInfo : public ResourceBarrierTypeInfoBase, public EnumTable(EResourceBarrierType, ResourceBarrierTypeInfoBase, EResourceBarrierType::Count, D3D12ResourceBarrierTypeInfo)
    {
        D3D12ResourceBarrierTypeInfo() = default;
        D3D12ResourceBarrierTypeInfo(const char* name, D3D12_RESOURCE_BARRIER_TYPE barrierType);
        static void initializePlatformTable();
    };
#pragma endregion
}
