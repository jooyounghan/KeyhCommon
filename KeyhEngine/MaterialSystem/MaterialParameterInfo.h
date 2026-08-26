#pragma once
#include "ReflectSerializer.h"

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
		Texture
	};

    constexpr uint32 kRefernecePathSize = 4;
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
        case MaterialParameterType::Texture:
            return kRefernecePathSize;
        default:
            return 0;
        }
    }

	class REFLECTIVE(MaterialParameterInfo)
	{
		KEYH_REFLECT_BODY(MaterialParameterInfo)

	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "Name")
		FlyweightStringA _parameterName;

		KEYH_REFLECT_PROPERTY(PropertyName = "Type")
		MaterialParameterType _parameterType;

		KEYH_REFLECT_PROPERTY(PropertyName = "Desc")
		StaticStringA _description;

		KEYH_REFLECT_PROPERTY(PropertyName = "DefaultValue")
		StaticStringA _defaultValue;

    public:
		const FlyweightStringA& getParameterName() const { return _parameterName; }
		inline uint32           getGpuMemorySize() const { return getParamTypeGpuMemorySize(_parameterType); }
	};
}
#include "MaterialParameterInfo.reflect_generated.inl"
