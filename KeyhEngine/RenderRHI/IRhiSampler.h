#pragma once

namespace keyh
{
	struct RhiStaticSamplerDesc
	{
		uint32				_shaderRegister = 0;
		uint32				_registerSpace = 0;
		EFilterType			_filter = EFilterType::MinMagMipLinear;
		ESamplerAddressMode _addressU = ESamplerAddressMode::Clamp;
		ESamplerAddressMode _addressV = ESamplerAddressMode::Clamp;
		ESamplerAddressMode _addressW = ESamplerAddressMode::Clamp;
		EComparisonFunction _comparisonFunc = EComparisonFunction::Never;
		EShaderStage		_stageFlags = EShaderStage::All;
	};

	class IRhiSampler
	{};
}

#pragma region FNV1aHash Specializations
namespace keyh
{
	DECLARE_FNV1A_HASH_SPECIALIZATION(RhiStaticSamplerDesc);
}
#pragma endregion
