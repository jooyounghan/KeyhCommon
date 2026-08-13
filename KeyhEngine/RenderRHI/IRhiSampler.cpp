#include "RenderRhiPch.h"
#include "IRhiSampler.h"

namespace keyh
{

}

#pragma region FNV1aHash Specializations
namespace keyh
{
	DEFINE_FNV1A_HASH_DEFAULT_SPECIALIZATION(RhiStaticSamplerDesc);
}
#pragma endregion