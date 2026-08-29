namespace keyh
{
	template<typename T>
	MaterialParameterHolder<T>::MaterialParameterHolder(T defaultValue) 
		: _value(keyh::move(defaultValue))
	{
	
	}

	template <>
	MaterialParameterType MaterialParameterHolder<int>::getType() const
	{
		return MaterialParameterType::Int;
	}

	template <>
	MaterialParameterType MaterialParameterHolder<int2>::getType() const
	{
		return MaterialParameterType::Int2;
	}

	template <>
	MaterialParameterType MaterialParameterHolder<float>::getType() const
	{
		return MaterialParameterType::Float;
	}

	template <>
	MaterialParameterType MaterialParameterHolder<float2>::getType() const
	{
		return MaterialParameterType::Float2;
	}

	template <>
	MaterialParameterType MaterialParameterHolder<float3>::getType() const
	{
		return MaterialParameterType::Float3;
	}

	template <>
	MaterialParameterType MaterialParameterHolder<float4>::getType() const
	{
		return MaterialParameterType::Float4;
	}
}