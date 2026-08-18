#include "RhiSystemPch.h"
#include "D3D12PipelineLayout.h"
#include "D3D12Device.h"

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

		//FNV1aHash< EPipelineLayoutFlags> hasher;
		return hash;
	}

	IRhiPipelineLayout::IRhiPipelineLayout(const RhiPipelineLayoutDesc& desc)
		: _hashValue(desc.getHash())
		, _bindingCount(desc._bindingCount)
		, _pushConstantCount(desc._pushConstantCount)
		, _staticSamplerCount(desc._staticSamplerCount)
	{

	}

	inline void clearDenyFlag(D3D12_ROOT_SIGNATURE_FLAGS& flags, uint32_t activeStages, EShaderStage stage, D3D12_ROOT_SIGNATURE_FLAGS denyFlag)
	{
		if (activeStages & static_cast<uint32_t>(stage))
		{
			flags &= ~denyFlag;
		}
	}

	D3D12_ROOT_SIGNATURE_FLAGS calculateD3D12RootSignatureFlags(const RhiPipelineLayoutDesc& desc)
	{
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS
			| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
			| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
			| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
			| D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS
			| D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS
			| D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS;

		uint32_t activeStages = 0;

		for (uint32_t i = 0; i < desc._bindingCount; ++i)
		{
			activeStages |= static_cast<uint32_t>(D3D12ShaderStageInfo::getInfo(desc._bindings[i]._stageFlags)._visibility);
		}

		for (uint32_t i = 0; i < desc._pushConstantCount; ++i)
		{
			activeStages |= static_cast<uint32_t>(D3D12ShaderStageInfo::getInfo(desc._pushConstants[i]._stageFlags)._visibility);
		}

		for (uint32_t i = 0; i < desc._staticSamplerCount; ++i)
		{
			activeStages |= static_cast<uint32_t>(D3D12ShaderStageInfo::getInfo(desc._staticSamplers[i]._stageFlags)._visibility);
		}

		clearDenyFlag(flags, activeStages, EShaderStage::Vertex, D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS);
		clearDenyFlag(flags, activeStages, EShaderStage::Hull, D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS);
		clearDenyFlag(flags, activeStages, EShaderStage::Domain, D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS);
		clearDenyFlag(flags, activeStages, EShaderStage::Geometry, D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);
		clearDenyFlag(flags, activeStages, EShaderStage::Pixel, D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS);
		clearDenyFlag(flags, activeStages, EShaderStage::Amplification, D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS);
		clearDenyFlag(flags, activeStages, EShaderStage::Mesh, D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS);

		if (activeStages & static_cast<uint32_t>(EShaderStage::Vertex))
		{
			flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		}

		return flags;
	}

	D3D12PipelineLayout::D3D12PipelineLayout(D3D12Device* device, const RhiPipelineLayoutDesc& desc)
		: IRhiPipelineLayout(desc)
	{	
		InlinedVector<D3D12_ROOT_PARAMETER, 64> rootParameters;
		uint32_t paramIndex = 0;
		for (uint32_t idx = 0; idx < desc._pushConstantCount; ++idx)
		{
			const RhiPushConstantRange& pushConst = desc._pushConstants[idx];
			D3D12_ROOT_PARAMETER& rootParameter = rootParameters.emplace_back();
			rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			rootParameter.Constants.ShaderRegister = pushConst._shaderRegister;
			rootParameter.Constants.RegisterSpace = pushConst._registerSpace;
			rootParameter.Constants.Num32BitValues = pushConst._sizeInBytes / 4;
			rootParameter.ShaderVisibility = D3D12ShaderStageInfo::getInfo(pushConst._stageFlags)._visibility;
			paramIndex++;
		}

		InlinedVector<D3D12_DESCRIPTOR_RANGE, 64> descriptorRanges;
		for (uint32_t idx = 0; idx < desc._bindingCount; ++idx)
		{
			const RhiDescriptorBinding& binding = desc._bindings[idx];
			D3D12_DESCRIPTOR_RANGE& descriptorRange = descriptorRanges.emplace_back();
			descriptorRange.RangeType = D3D12DescriptorTypeInfo::getInfo(binding._descriptorType)._type;
			descriptorRange.NumDescriptors = binding._descriptorCount;
			descriptorRange.BaseShaderRegister = binding._bindingSlot;
			descriptorRange.RegisterSpace = binding._registerSpace;
			descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			D3D12_ROOT_PARAMETER& rootParameter = rootParameters.emplace_back();
			rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameter.DescriptorTable.NumDescriptorRanges = 1;
			rootParameter.DescriptorTable.pDescriptorRanges = &descriptorRanges[idx];
			rootParameter.ShaderVisibility = D3D12ShaderStageInfo::getInfo(binding._stageFlags)._visibility;
			paramIndex++;
		}

		InlinedVector<D3D12_STATIC_SAMPLER_DESC, 64> staticSamplers;
		for (uint32_t idx = 0; idx < desc._staticSamplerCount; ++idx)
		{
			const RhiStaticSamplerDesc& samplerDesc = desc._staticSamplers[idx];
			D3D12_STATIC_SAMPLER_DESC& staticSampler = staticSamplers.emplace_back();
			staticSampler.Filter = D3D12FilterInfo::getInfo(samplerDesc._filter)._filter;
			staticSampler.AddressU = D3D12SamplerAddressModeInfo::getInfo(samplerDesc._addressU)._addressMode;
			staticSampler.AddressV = D3D12SamplerAddressModeInfo::getInfo(samplerDesc._addressV)._addressMode;
			staticSampler.AddressW = D3D12SamplerAddressModeInfo::getInfo(samplerDesc._addressW)._addressMode;
			staticSampler.MipLODBias = 0.0f;
			staticSampler.MaxAnisotropy = 16;
			staticSampler.ComparisonFunc = D3D12ComparisonFunctionInfo::getInfo(samplerDesc._comparisonFunc)._comparisonFunc;
			staticSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
			staticSampler.MinLOD = 0.0f;
			staticSampler.MaxLOD = D3D12_FLOAT32_MAX;
			staticSampler.ShaderRegister = samplerDesc._shaderRegister;
			staticSampler.RegisterSpace = samplerDesc._registerSpace;
			staticSampler.ShaderVisibility = D3D12ShaderStageInfo::getInfo(samplerDesc._stageFlags)._visibility;
		}

		uint32_t totalParamsCount = _bindingCount + _pushConstantCount;
		D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
		rootSigDesc.NumParameters = totalParamsCount;
		rootSigDesc.pParameters = rootParameters.data();
		rootSigDesc.NumStaticSamplers = desc._staticSamplerCount;
		rootSigDesc.pStaticSamplers = staticSamplers.data();
		rootSigDesc.Flags = calculateD3D12RootSignatureFlags(desc);

		Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
		HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

		if (SUCCEEDED(hr))
		{
			device->getNativeDevice()->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(_rootSignature.GetAddressOf()));
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