#include "RhiSystemPch.h"
#include "IRhiSampler.h"

namespace keyh
{
	IRhiSampler::IRhiSampler(const RhiStaticSamplerDesc& desc)
	{
		FNV1aHash<RhiStaticSamplerDesc> hasher;
		_hashValue = hasher(desc);
	}
}

#pragma region FNV1aHash Specializations
namespace keyh
{
	DEFINE_FNV1A_HASH_DEFAULT_SPECIALIZATION(RhiStaticSamplerDesc);
}
#pragma endregion