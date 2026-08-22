#include "RhiSystemPch.h"
#include "D3D12GlobalResourceHeap.h"
#include "D3D12Device.h"

namespace keyh
{
	D3D12GlobalResourceHeap::D3D12GlobalResourceHeap(D3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32 capacity)
	{
		ID3D12Device* nativeDevice = device->getNativeDevice();

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = heapType;
		heapDesc.NumDescriptors = capacity;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;

		nativeDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_heap));

		_descriptorSize = nativeDevice->GetDescriptorHandleIncrementSize(heapType);
		_cpuStart = _heap->GetCPUDescriptorHandleForHeapStart();
		_gpuStart = _heap->GetGPUDescriptorHandleForHeapStart();

		_freeIndices.reserve(kFreeHeapIndicesCapacity);
	}

	bool D3D12GlobalResourceHeap::allocate(D3D12_CPU_DESCRIPTOR_HANDLE& outCpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& outGpuHandle, uint32& outIndex)
	{
		if (_freeIndices.empty() == false)
		{
			outIndex = _freeIndices.unsafeBack();
			_freeIndices.pop_back();
		}
		else if (_allocatedCount < _capacity)
		{
			outIndex = _allocatedCount++;
		}
		else
		{
			KEYH_ASSERT(false, "D3D12GlobalResourceHeap: No more free descriptors available.");
			return false;
		}

		outCpuHandle.ptr = _cpuStart.ptr + static_cast<SIZE_T>(outIndex * _descriptorSize);
		outGpuHandle.ptr = _gpuStart.ptr + static_cast<UINT64>(outIndex * _descriptorSize);

		return true;
	}

	void D3D12GlobalResourceHeap::free(uint32 index)
	{
		if (index < _capacity)
		{
			_freeIndices.push_back(index);
		}
		else
		{
			KEYH_ASSERT(false, "D3D12GlobalResourceHeap: Invalid index to free.");
		}
	}
}

