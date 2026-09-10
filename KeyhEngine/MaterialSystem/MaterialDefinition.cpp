#include "MaterialSystemPch.h"
#include "MaterialDefinition.h"
#include "MaterialParameterDefinition.h"
#include "StrUtil.h"

namespace keyh
{
	KEYH_REFLECT_DEFINE_BODY(MaterialDefinition)

	static void deserializeMaterialParameterComponent(const StringViewA& token, bool isFloat, byte* dest)
	{
		if (isFloat)
		{
			const float value = StrUtil::strToFloat<float>(token.c_str());
			memcpy(dest, &value, sizeof(value));
		}
		else
		{
			const int32 value = StrUtil::strToInt<int32>(token.c_str());
			memcpy(dest, &value, sizeof(value));
		}
	}

	static void deserializeMaterialParameterValue(const MaterialParameterDefinition* materialParameterDef, byte* dest)
	{
		const MaterialParameterType paramType = materialParameterDef->_parameterType;
		if (isMaterialDefaultParameterDeserializable(paramType) == false )
			return;

		StringViewA defaultValueView(materialParameterDef->_defaultValue.c_str(), materialParameterDef->_defaultValue.length());
		SplitterStringA splitter(defaultValueView, ",");
		const uint32 tokenCount = static_cast<uint32>(splitter.size());
		const uint32 componentCount = getParamTypeComponentCount(paramType);
		KEYH_ASSERT_DEV(tokenCount == componentCount, "Default value component count does not match MaterialParameterType");

		const bool isFloat = isFloatParamType(paramType);
		const uint32 componentSize = isFloat ? sizeof(float) : sizeof(int32);

		for (uint32 idx = 0; idx < componentCount; ++idx)
		{
			deserializeMaterialParameterComponent(splitter[idx], isFloat, dest + idx * componentSize);
		}
	}

	void MaterialDefinition::initialize()
	{
		const size_t bitFlagByteSize = MemoryUtil::align(_materialBitFlagDefinitions.size(), 8) / 8;
		uint32 totalSize = static_cast<uint32>(MemoryUtil::align(bitFlagByteSize, 4));

		// Convert MaterialBitFlagDefinitions To MaterialBitFlagViewDescs
		for (const MaterialBitFlagDefinition* materialBitFlagDef : _materialBitFlagDefinitions)
		{
			_materialBitFlagViewDescs.emplace_back(materialBitFlagDef->_parameterName, materialBitFlagDef->_bitIndex);
		}

		for (const MaterialParameterDefinition* materialParameterDef : _materialParameterDefinitions)
		{
			const uint32 paramSize = materialParameterDef->getGpuMemorySize();
			if ((totalSize & 15) + paramSize > 16)
			{
				totalSize = MemoryUtil::align(totalSize, 16);
			}

			// Convert MAterialParameterDefinitions To MaterialParameterViewDescs
			_materialParameterViewDescs.emplace_back(
				materialParameterDef->_parameterName
				, materialParameterDef->_parameterType
				, totalSize
				, paramSize
			);

			totalSize += paramSize;
		}
		totalSize = MemoryUtil::align(totalSize, 16);

		// Set gpuDefaultMemoryBlock with Default Values
		_gpuDefaultMemoryBlock.allocate(totalSize);
		byte* gpuDefaultMemoryBlockBuffer = _gpuDefaultMemoryBlock.getBuffer();
		memset(gpuDefaultMemoryBlockBuffer, 0, totalSize);

		for (const MaterialBitFlagDefinition* materialBitFlagDef : _materialBitFlagDefinitions)
		{
			uint32 byteIndex = materialBitFlagDef->_bitIndex / 8;
			const byte bitMask = static_cast<byte>(materialBitFlagDef->_defaultValue) << (materialBitFlagDef->_bitIndex % 8);
			gpuDefaultMemoryBlockBuffer[byteIndex] |= bitMask;
		}

		const uint32 materialParameterViewDescsSize = static_cast<uint32>(_materialParameterViewDescs.size());
		KEYH_ASSERT_DEV(materialParameterViewDescsSize == _materialParameterDefinitions.size(), "MaterialParameterViewDescs and MaterialParameterDefinitions size mismatch");

		for (uint32 idx = 0; idx < materialParameterViewDescsSize; ++idx)
		{
			const MaterialParameterDefinition* materialParameterDef = _materialParameterDefinitions[idx];
			const MaterialParameterViewDesc& paramViewDesc = _materialParameterViewDescs[idx];

			void* dest = gpuDefaultMemoryBlockBuffer + paramViewDesc._offset;
			deserializeMaterialParameterValue(materialParameterDef, static_cast<byte*>(dest));
		}
	}
}
