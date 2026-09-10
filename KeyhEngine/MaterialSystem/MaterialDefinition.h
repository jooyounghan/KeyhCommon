#pragma once

namespace keyh
{
	class MaterialBitFlagDefinition;
	class MaterialParameterDefinition;

	struct MaterialBitFlagViewDesc
	{
		FlyweightStringA	_materialBitFlagName;
		uint32				_bitOffset;
	};

	struct MaterialParameterViewDesc
	{
		FlyweightStringA		_materialParameterName;
		MaterialParameterType	_parameterType;	
		uint32					_offset;
		uint32					_size;
	};

	class REFLECTIVE(MaterialDefinition)
	{
		KEYH_REFLECT_DECLARE_BODY(MaterialDefinition)

	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialName")
		FlyweightStringA _materialName;

		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialBitFlagDefinitions")
		OwnerVector<MaterialBitFlagDefinition> _materialBitFlagDefinitions;

		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialParameterDefinitions")
		OwnerVector<MaterialParameterDefinition> _materialParameterDefinitions;

	private:
		Vector<MaterialBitFlagViewDesc>		_materialBitFlagViewDescs;
		Vector<MaterialParameterViewDesc>	_materialParameterViewDescs;
		DynamicBuffer<byte>					_gpuDefaultMemoryBlock;

	public:
		inline const Vector<MaterialBitFlagViewDesc>&		getMaterialBitFlagViewDescs() const { return _materialBitFlagViewDescs; }
		inline const Vector< MaterialParameterViewDesc>&	getMaterialParameterViewDescs() const { return _materialParameterViewDescs; }

	public:
		void initialize();
	};
}

#include "MaterialDefinition.reflect_generated.inl"
