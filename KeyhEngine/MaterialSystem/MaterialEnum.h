#pragma once

namespace keyh
{
	KEYH_REFLECT_ENUM
		enum class MaterialParameterType : uint8
	{
		Int,
		Int2,
		Float,
		Float2,
		Float3,
		Float4,
		Transform,
		ReferencedPath_Texture,
	};

    constexpr uint32 kReferencePathSize = sizeof(uint32);
	constexpr uint32 kReferencePathMaxLength = 256;
    constexpr uint32 getParamTypeGpuMemorySize(MaterialParameterType type)
    {
        switch (type)
        {
        case MaterialParameterType::Int:
            return 4;
        case MaterialParameterType::Int2:
            return 8;
        case MaterialParameterType::Float:
            return 4;
        case MaterialParameterType::Float2:
            return 8;
        case MaterialParameterType::Float3:
            return 12;
        case MaterialParameterType::Float4:
            return 16;
        case MaterialParameterType::Transform:
            return 64;
        case MaterialParameterType::ReferencedPath_Texture:
            return kReferencePathSize;
        default:
            return 0;
        }
    }
}

#include "MaterialEnum.reflect_generated.inl"
