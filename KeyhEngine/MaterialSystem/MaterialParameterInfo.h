#pragma once
#include "MaterialEnum.h"

namespace keyh
{
	class IMaterialParameterHolder;

	class REFLECTIVE(MaterialParameterInfo)
	{
		KEYH_REFLECT_DECLARE_BODY(MaterialParameterInfo)

	public:
		virtual ~MaterialParameterInfo() override;

	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "Name")
		FlyweightStringA _parameterName;

		KEYH_REFLECT_PROPERTY(PropertyName = "Type")
		MaterialParameterType _parameterType;

		KEYH_REFLECT_PROPERTY(PropertyName = "Desc")
		StaticStringA _description;

		KEYH_REFLECT_PROPERTY(PropertyName = "DefaultValue")
		StaticStringA _defaultValue;

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
