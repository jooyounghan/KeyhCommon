#include "MaterialSystemPch.h"
#include "MaterialInfo.h"
#include "MaterialParameterInfo.h"

namespace keyh
{
	MaterialParameterBinding::MaterialParameterBinding(
		uint32 offset
		, const IMaterialParameterHolder* holder
	)
		: _offset_bitIndex(offset << 8)
		, _parameterHolder(holder)
	{
	
	}

	MaterialParameterBinding::MaterialParameterBinding(
		uint32 offset
		, uint32 bitIndex
		, const IMaterialParameterHolder* holder
	)
		: _offset_bitIndex((offset << 8) | (bitIndex & 0xFF))
		, _parameterHolder(holder)
	{
	
	}

	KEYH_REFLECT_DEFINE_BODY(MaterialInfo)

	void MaterialInfo::initialize()
	{
		_parameterBindings.clear();

		uint32 totalSize = 0;
		for (const MaterialParameterInfo* paramInfo : _materialParameterInfos)
		{
			const uint32 paramSize = paramInfo->getGpuMemorySize();
			const uint32 usedInChunk = totalSize & 15;
			const uint32 spaceLeft = (16 - usedInChunk) & 15;

			if (usedInChunk != 0 && paramSize > spaceLeft)
			{
				totalSize += spaceLeft;
			}

			_parameterBindings.insert(paramInfo->getParameterName(), MaterialParameterBinding{ totalSize, paramInfo->getParameterHolder()});
			totalSize += paramSize;
		}
	}
	
	const MaterialParameterBinding* MaterialInfo::getParameterBinding(const FlyweightStringA& parameterName) const
	{
		MaterialParameterBindingMap::ConstFindResult findResult = _parameterBindings.find(parameterName);
		return  findResult.isFound() ? findResult.value() : nullptr;
	}
}