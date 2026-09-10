//namespace keyh
//{
//	template<typename T>
//	MaterialParameterHolder<T>::MaterialParameterHolder(T defaultValue) 
//		: _value(keyh::move(defaultValue))
//	{
//	
//	}
//
//	template <>
//	inline MaterialParameterType MaterialParameterHolder<int>::getType() const
//	{
//		return MaterialParameterType::Int;
//	}
//
//	template <>
//	inline MaterialParameterType MaterialParameterHolder<int2>::getType() const
//	{
//		return MaterialParameterType::Int2;
//	}
//
//	template <>
//	inline MaterialParameterType MaterialParameterHolder<float>::getType() const
//	{
//		return MaterialParameterType::Float;
//	}
//
//	template <>
//	inline MaterialParameterType MaterialParameterHolder<float2>::getType() const
//	{
//		return MaterialParameterType::Float2;
//	}
//
//	template <>
//	inline MaterialParameterType MaterialParameterHolder<float3>::getType() const
//	{
//		return MaterialParameterType::Float3;
//	}
//
//	template <>
//	inline MaterialParameterType MaterialParameterHolder<float4>::getType() const
//	{
//		return MaterialParameterType::Float4;
//	}
//}