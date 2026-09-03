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
	public:
		MaterialParameterBinding(uint32 offset, const IMaterialParameterHolder* holder);
		MaterialParameterBinding(uint32 offset, uint32 bitIndex, const IMaterialParameterHolder* holder);

	private:
		uint32								_offset_bitIndex = 0;
		const IMaterialParameterHolder*		_parameterHolder = nullptr;

	public:
		uint32							getOffset() const { return _offset_bitIndex >> 8; }
		uint32							getBitIndex() const { return _offset_bitIndex & 0xFF; }
		const IMaterialParameterHolder* getParameterHolder() const { return _parameterHolder; }
	};

	using MaterialParameterBindingMap = HashMap<FlyweightStringA, MaterialParameterBinding>;

	class REFLECTIVE(MaterialInfo)
	{
		KEYH_REFLECT_DECLARE_BODY(MaterialInfo)

	public:
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
