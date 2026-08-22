#pragma once

namespace keyh
{
	class D3D12Device;

	class D3D12GlobalResourceHeap
	{
		static constexpr uint32 kMaxDescriptorHeapCapacity = 1000000;
		static constexpr uint32 kFreeHeapIndicesCapacity = 1024;

	public:
		D3D12GlobalResourceHeap(D3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32 capacity = kMaxDescriptorHeapCapacity);
		~D3D12GlobalResourceHeap() = default;

	public:
		bool allocate(D3D12_CPU_DESCRIPTOR_HANDLE& outCpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& outGpuHandle, uint32& outIndex);
		void free(uint32 index);

	public:
		inline ID3D12DescriptorHeap*	getNativeHeap() const { return _heap.Get(); }
		inline uint32					getDescriptorSize() const { return _descriptorSize; }


	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	_heap;
		D3D12_CPU_DESCRIPTOR_HANDLE						_cpuStart = {};
		D3D12_GPU_DESCRIPTOR_HANDLE						_gpuStart = {};
		uint32											_descriptorSize = 0;
		uint32											_capacity = 0;
		uint32											_allocatedCount = 0;
		Vector<uint32>									_freeIndices;
	};
}

