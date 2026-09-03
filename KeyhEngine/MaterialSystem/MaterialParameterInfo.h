#pragma once
#include "MaterialEnum.h"

namespace keyh
{
	class IMaterialParameterHolder;

	class REFLECTIVE(MaterialBitFlagParameterInfo)
	{
		KEYH_REFLECT_DECLARE_BODY(MaterialBitFlagParameterInfo)

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

	class REFLECTIVE(MaterialParameterInfo)
	{
		KEYH_REFLECT_DECLARE_BODY(MaterialParameterInfo)

	public:
		KEYH_REFLECT_PROPERTY(PropertyName = "Name")
		FlyweightStringA _parameterName;

		KEYH_REFLECT_PROPERTY(PropertyName = "Type")
		MaterialParameterType _parameterType;

		KEYH_REFLECT_PROPERTY(PropertyName = "DefaultValue")
		StaticStringA _defaultValue;

		KEYH_REFLECT_PROPERTY(PropertyName = "Desc")
		StaticStringA _description;

		KEYH_REFLECT_PROPERTY(PropertyName = "BitFlagInfo")
		Vector<MaterialBitFlagParameterInfo> _bitFlagInfos;

	protected:
		Ptr<IMaterialParameterHolder> _parameterHolder;

	public:
		void initializeParameterHolder();

    public:
		inline const FlyweightStringA&		getParameterName() const { return _parameterName; }
		inline uint32						getGpuMemorySize() const { return getParamTypeGpuMemorySize(_parameterType); }
		inline IMaterialParameterHolder*	getParameterHolder() const { return _parameterHolder.get(); }
	};
}
#include "MaterialParameterInfo.reflect_generated.inl"
