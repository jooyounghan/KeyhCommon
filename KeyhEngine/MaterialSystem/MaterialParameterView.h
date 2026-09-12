#pragma once
#include "MaterialEnum.h"

namespace keyh
{
	class MaterialParameterUpdateDesc
	{
	private:
		MaterialParameterUpdateDesc() = default;

	public:
		~MaterialParameterUpdateDesc() = default;

	public:
		static MaterialParameterUpdateDesc createIntUpdateDesc(const FlyweightStringA& parameterName, const int* data);
		static MaterialParameterUpdateDesc createInt2UpdateDesc(const FlyweightStringA& parameterName, const int2* data);
		static MaterialParameterUpdateDesc createFloatUpdateDesc(const FlyweightStringA& parameterName, const float* data);
		static MaterialParameterUpdateDesc createFloat2UpdateDesc(const FlyweightStringA& parameterName, const float2* data);
		static MaterialParameterUpdateDesc createFloat3UpdateDesc(const FlyweightStringA& parameterName, const float3* data);
		static MaterialParameterUpdateDesc createFloat4UpdateDesc(const FlyweightStringA& parameterName, const float4* data);

	private:
		FlyweightStringA		_parameterName = FlyweightStringA::Empty;
		MaterialParameterType	_parameterType = MaterialParameterType::Unknown;
		const void*				_data = nullptr;

	public:
		inline const FlyweightStringA&	getParameterName() const { return _parameterName; }
		inline MaterialParameterType	getParameterType() const { return _parameterType; }
		inline const void*				getData() const { return _data; }
	};

	class REFLECTIVE(MaterialParameterView)
	{
		KEYH_REFLECT_DECLARE_BODY(MaterialParameterView)

#ifdef KEYH_DEV
			friend class MaterialDefinition;
#endif

	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "Type")
		MaterialParameterType _parameterType;

		KEYH_REFLECT_PROPERTY(PropertyName = "Offset")
		uint32 _offset;

	public:
		inline MaterialParameterType	getParameterType() const { return _parameterType; }
		inline uint32					getOffset() const { return _offset; }
	};
}

#include "MaterialParameterView.reflect_generated.inl"
