#include "RenderRhiPch.h"
#include "IRhiPipelineLayout.h"

namespace keyh
{
	size_t RhiPipelineLayoutDesc::getHash() const 
	{
		size_t hash = 0;
		{
			FNV1aHash<uint32> hasher;
			hash ^= hasher(_bindingCount);
			hash ^= hasher(_pushConstantCount);
			hash ^= hasher(_staticSamplerCount);
		}
		
		{
			FNV1aHash<RhiDescriptorBinding> hasher;
			for (uint32 i = 0; i < _bindingCount; ++i)
			{
				hash ^= hasher(_bindings[i]);
			}
		}

		{
			FNV1aHash<RhiPushConstantRange> hasher;
			for (uint32 i = 0; i < _pushConstantCount; ++i)
			{
				hash ^= hasher(_pushConstants[i]);
			}
		}

		{
			FNV1aHash<RhiStaticSamplerDesc> hasher;
			for (uint32 i = 0; i < _staticSamplerCount; ++i)
			{
				hash ^= hasher(_staticSamplers[i]);
			}
		}

		{
			FNV1aHash<RhiStaticSamplerDesc> hasher;
			for (uint32 i = 0; i < _staticSamplerCount; ++i)
			{
				hash ^= hasher(_staticSamplers[i]);
			}
		}

		return hash;
	}

	IRhiPipelineLayout::IRhiPipelineLayout(const RhiPipelineLayoutDesc& desc)
		: _hashValue(desc.getHash())
		, _bindingCount(desc._bindingCount)
		, _pushConstantCount(desc._pushConstantCount)
		, _staticSamplerCount(desc._staticSamplerCount)
	{

	}

	D3D12PipelineLayout::D3D12PipelineLayout(ID3D12Device* device, const RhiPipelineLayoutDesc& desc)
		: IRhiPipelineLayout(desc)
	{	
		Vector<D3D12_ROOT_PARAMETER> rootParameters;
		rootParameters.resize(_pushConstantCount);
		uint32_t paramIndex = 0;
		for (uint32_t idx = 0; idx < desc._pushConstantCount; ++idx)
		{
			const RhiPushConstantRange& pushConst = desc._pushConstants[i];
			rootParameters[paramIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			rootParameters[paramIndex].Constants.ShaderRegister = pushConst._shaderRegister;
			rootParameters[paramIndex].Constants.RegisterSpace = pushConst._registerSpace;
			rootParameters[paramIndex].Constants.Num32BitValues = pushConst._sizeInBytes / 4;
			rootParameters[paramIndex].ShaderVisibility = D3D12ShaderStageInfo::getInfo(pushConst._stageFlags)._visibility;
			paramIndex++;
		}

		Vector<D3D12_DESCRIPTOR_RANGE> descriptorRanges;
		descriptorRanges.resize(desc._bindingCount);
		for (uint32_t idx = 0; idx < desc._bindingCount; ++idx)
		{
			const RhiDescriptorBinding& binding = desc._bindings[idx];

			descriptorRanges[idx].RangeType = D3D12DescriptorTypeInfo::getInfo(binding._descriptorType)._type;
			descriptorRanges[idx].NumDescriptors = binding._descriptorCount;
			descriptorRanges[idx].BaseShaderRegister = binding._bindingSlot;
			descriptorRanges[idx].RegisterSpace = binding._registerSpace;
			descriptorRanges[idx].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			rootParameters[paramIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[paramIndex].DescriptorTable.NumDescriptorRanges = 1;
			rootParameters[paramIndex].DescriptorTable.pDescriptorRanges = &descriptorRanges[idx];
			rootParameters[paramIndex].ShaderVisibility = D3D12ShaderStageInfo::getInfo(binding._stageFlags)._visibility;
			paramIndex++;
		}

		Vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers;
		staticSamplers.resize(desc._staticSamplerCount);
		for (uint32_t idx = 0; idx < desc._staticSamplerCount; ++idx)
		{
			const RhiStaticSamplerDesc& samplerDesc = desc._staticSamplers[idx];
			staticSamplers[idx].Filter = D3D12FilterInfo::getInfo(samplerDesc._filter)._filter;
			staticSamplers[idx].AddressU = D3D12SamplerAddressModeInfo::getInfo(samplerDesc._addressU)._addressMode;
			staticSamplers[idx].AddressV = D3D12SamplerAddressModeInfo::getInfo(samplerDesc._addressV)._addressMode;
			staticSamplers[idx].AddressW = D3D12SamplerAddressModeInfo::getInfo(samplerDesc._addressW)._addressMode;
			staticSamplers[idx].MipLODBias = 0.0f;
			staticSamplers[idx].MaxAnisotropy = 16;
			staticSamplers[idx].ComparisonFunc = D3D12ComparisonFunctionInfo::getInfo(samplerDesc._comparisonFunc)._comparisonFunc;
			staticSamplers[idx].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
			staticSamplers[idx].MinLOD = 0.0f;
			staticSamplers[idx].MaxLOD = D3D12_FLOAT32_MAX;
			staticSamplers[idx].ShaderRegister = samplerDesc._shaderRegister;
			staticSamplers[idx].RegisterSpace = samplerDesc._registerSpace;
			staticSamplers[idx].ShaderVisibility = D3D12ShaderStageInfo::getInfo(samplerDesc._stageFlags)._visibility;
		}

		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
		if (desc.flags & static_cast<uint32_t>(ERhiPipelineLayoutFlags::AllowInputAssemblerInputLayout))
		{
			flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		}

		uint32_t totalParamsCount = _bindingCount + _pushConstantCount;
		D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
		rootSigDesc.NumParameters = totalParamsCount;
		rootSigDesc.pParameters = rootParameters.data();
		rootSigDesc.NumStaticSamplers = desc._staticSamplerCount;
		rootSigDesc.pStaticSamplers = staticSamplers.data();
		rootSigDesc.Flags = flags;

		Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
		HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

		if (SUCCEEDED(hr))
		{
			device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(_rootSignature.GetAddressOf()));
		}
	}

}

#pragma region FNV1aHash Specializations
namespace keyh
{
	DEFINE_FNV1A_HASH_DEFAULT_SPECIALIZATION(RhiDescriptorBinding);
	DEFINE_FNV1A_HASH_DEFAULT_SPECIALIZATION(RhiPushConstantRange);
}
#pragma endregion