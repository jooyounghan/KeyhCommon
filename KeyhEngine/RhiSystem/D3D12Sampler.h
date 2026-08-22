#pragma once
#include "IRhiSampler.h"

namespace keyh
{
	class D3D12Device;

	class D3D12Sampler : public IRhiSampler
	{
	public:
		D3D12Sampler(D3D12Device* device, const RhiStaticSamplerDesc& desc);
		virtual ~D3D12Sampler() override = default;

	public:
		inline D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle() const { return _cpuHandle; }
		inline D3D12_GPU_DESCRIPTOR_HANDLE getGpuHandle() const { return _gpuHandle; }


	private:
		D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandle = {};
		uint32						_descriptorHeapAllocIndex = kInvalidUint32;
	};
}