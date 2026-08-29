#pragma once

namespace keyh
{
	class MaterialParameterInfo;
	class IMaterialParameterHolder;
	// MaterialInfo의 역할
	// .material 파일을 읽어서 매터리얼에서 사용할 Bindless Constants를 구성하는 역할
	// 이름으로 접근하여 해당 파라미터가 버퍼의 시작부터 몇 바이트에 위치하는지, 몇 바이트를 차지하는지, 타입은 무엇인지 등을 제공

	struct MaterialParameterBinding
	{
		uint32								_offset = 0;
		const IMaterialParameterHolder*		_parameterHolder = nullptr;
	};

	using MaterialParameterBindingMap = HashMap<FlyweightStringA, MaterialParameterBinding>;

	class REFLECTIVE(MaterialInfo)
	{
		KEYH_REFLECT_BODY(MaterialInfo)

	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialParameterInfos")
		OwnerVector<MaterialParameterInfo> _materialParameterInfos;

	private:
		MaterialParameterBindingMap	_parameterBindings;
		uint32						_totalGpuMemorySize = 0;

	public:
		void initialize();

	public:
		const MaterialParameterBinding*	getParameterBinding(const FlyweightStringA & parameterName) const;
		inline uint32					getTotalGpuMemorySize() const { return _totalGpuMemorySize; }

	};
}
#include "MaterialInfo.reflect_generated.inl"
