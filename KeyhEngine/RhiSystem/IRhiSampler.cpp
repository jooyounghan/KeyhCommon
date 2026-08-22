#include "RhiSystemPch.h"
#include "IRhiSampler.h"
#include "D3D12Sampler.h"
#include "D3D12RhiConvert.h"
#include "D3D12Device.h"
#include "D3D12GlobalResourceHeap.h"

namespace keyh
{
	IRhiSampler::IRhiSampler(const RhiStaticSamplerDesc& desc)
	{
		FNV1aHash<RhiStaticSamplerDesc> hasher;
		_hashValue = hasher(desc);
	}

	D3D12Sampler::D3D12Sampler(D3D12Device* device, const RhiStaticSamplerDesc& desc)
		: IRhiSampler(desc)
	{
		device->getSamplerHeap()->allocate(_cpuHandle, _gpuHandle, _descriptorHeapAllocIndex);

		D3D12_SAMPLER_DESC d3d12SamplerDesc = toD3D12SamplerDesc(desc);
		device->getNativeDevice()->CreateSampler(
			&d3d12SamplerDesc,
			_cpuHandle
		);
	}
}

#pragma region FNV1aHash Specializations
namespace keyh
{
	DEFINE_FNV1A_HASH_DEFAULT_SPECIALIZATION(RhiStaticSamplerDesc);
}
#pragma endregion