#pragma once

namespace keyh
{
	KEYH_REFLECT_ENUM
	enum class MaterialParameterType : uint8
	{
		Unknown = 0,
		Int,
		Int2,
		Float,
		Float2,
		Float3,
		Float4,
		ReferencedPath_Texture,
	};

    constexpr uint32 kReferencePathSize = sizeof(uint32);
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
        case MaterialParameterType::ReferencedPath_Texture:
            return kReferencePathSize;
        default:
            KEYH_ASSERT(false, "Need to handle all MaterialParameterType cases");
            return 0;
        }
    }

	constexpr bool isMaterialDefaultParameterDeserializable(MaterialParameterType type)
	{
		switch (type)
		{
		case MaterialParameterType::Int:
		case MaterialParameterType::Int2:
		case MaterialParameterType::Float:
		case MaterialParameterType::Float2:
		case MaterialParameterType::Float3:
		case MaterialParameterType::Float4:
			return true;
		case MaterialParameterType::ReferencedPath_Texture:
			return false;
		default:
			KEYH_ASSERT(false, "Need to handle all MaterialParameterType cases");
			return false;
		}
	}

	// Number of scalar components that make up the default-value string (e.g. "1,2" for Float2).
	constexpr uint32 getParamTypeComponentCount(MaterialParameterType type)
	{
		switch (type)
		{
		case MaterialParameterType::Int:
		case MaterialParameterType::Float:
			return 1;
		case MaterialParameterType::Int2:
		case MaterialParameterType::Float2:
			return 2;
		case MaterialParameterType::Float3:
			return 3;
		case MaterialParameterType::Float4:
			return 4;
		case MaterialParameterType::ReferencedPath_Texture:
		default:
			KEYH_ASSERT(false, "Need to handle all deserializable MaterialParameterType cases");
			return 0;
		}
	}

	constexpr bool isFloatParamType(MaterialParameterType type)
	{
		switch (type)
		{
		case MaterialParameterType::Int:
		case MaterialParameterType::Int2:
			return false;
		case MaterialParameterType::Float:
		case MaterialParameterType::Float2:
		case MaterialParameterType::Float3:
		case MaterialParameterType::Float4:
			return true;
		case MaterialParameterType::ReferencedPath_Texture:
		default:
			KEYH_ASSERT(false, "Need to handle all deserializable MaterialParameterType cases");
			return false;
		}
	}
}

#include "MaterialEnum.reflect_generated.inl"
