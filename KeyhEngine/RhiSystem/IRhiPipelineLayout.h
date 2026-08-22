#pragma once
#include "IRhiSampler.h"

namespace keyh
{
	struct RhiDescriptorBinding
	{
		uint32				_bindingSlot = 0;
		uint32				_registerSpace = 0;
		EDescriptorType		_descriptorType = EDescriptorType::UniformBuffer;
		uint32				_descriptorCount = 1;
		EShaderStage		_stageFlags = EShaderStage::All;
	};

	struct RhiPushConstantRange
	{
		uint32				_shaderRegister = 0;
		uint32				_registerSpace = 0;
		uint32				_sizeInBytes = 0;
		EShaderStage		_stageFlags = EShaderStage::All;
	};

	struct RhiPipelineLayoutDesc
	{
		const RhiDescriptorBinding*		_bindings = nullptr;
		uint32							_bindingCount = 0;

		const RhiPushConstantRange*		_pushConstants = nullptr;
		uint32							_pushConstantCount = 0;

		const RhiStaticSamplerDesc*		_staticSamplers = nullptr;
		uint32							_staticSamplerCount = 0;

	public:
		size_t getHash() const;
	};

	class IRhiPipelineLayout
	{
	public:
		IRhiPipelineLayout(const RhiPipelineLayoutDesc& desc);
		virtual ~IRhiPipelineLayout() = default;

	public:
		inline size_t getHash() const { return _hashValue; }
		inline uint32 getBindingCount() const { return _bindingCount; };
		inline uint32 getPushConstantCount() const { return _pushConstantCount; }
		inline uint32 getStaticSamplerCount() const { return _staticSamplerCount; }

	protected:
		const size_t _hashValue = 0;
		const uint32 _bindingCount = 0;
		const uint32 _pushConstantCount = 0;
		const uint32 _staticSamplerCount = 0;
	};
}

#pragma region FNV1aHash Specializations
namespace keyh
{
	DECLARE_FNV1A_HASH_SPECIALIZATION(RhiDescriptorBinding);
	DECLARE_FNV1A_HASH_SPECIALIZATION(RhiPushConstantRange);
}
#pragma endregion
