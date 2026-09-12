#pragma once
#include "MaterialEnum.h"

namespace keyh
{
	class IMaterialParameterHolder;

	class REFLECTIVE(MaterialBitFlagDefinition)
	{
		KEYH_REFLECT_DECLARE_BODY(MaterialBitFlagDefinition)

#ifdef KEYH_DEV
			friend class MaterialManager;
#endif

	public:
		KEYH_REFLECT_PROPERTY(PropertyName = "Name")
			FlyweightStringA _parameterName;
		KEYH_REFLECT_PROPERTY(PropertyName = "BitIndex")
			uint8 _bitIndex;
		KEYH_REFLECT_PROPERTY(PropertyName = "DefaultValue")
			bool _defaultValue;
		KEYH_REFLECT_PROPERTY(PropertyName = "Desc")
			StaticStringA _description;
	};

	class REFLECTIVE(MaterialParameterDefinition)
	{
		KEYH_REFLECT_DECLARE_BODY(MaterialParameterDefinition)

#ifdef KEYH_DEV
			friend class MaterialManager;
#endif
	public:
		KEYH_REFLECT_PROPERTY(PropertyName = "Name")
		FlyweightStringA _parameterName;

		KEYH_REFLECT_PROPERTY(PropertyName = "Type")
		MaterialParameterType _parameterType;

		KEYH_REFLECT_PROPERTY(PropertyName = "DefaultValue")
		StaticStringA _defaultValue;

		KEYH_REFLECT_PROPERTY(PropertyName = "Desc")
		StaticStringA _description;

    public:
		inline const FlyweightStringA&		getParameterName() const { return _parameterName; }
		inline uint32						getGpuMemorySize() const { return getParamTypeGpuMemorySize(_parameterType); }
	};
}
#include "MaterialParameterDefinition.reflect_generated.inl"
