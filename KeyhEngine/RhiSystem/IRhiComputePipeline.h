#pragma once

namespace keyh
{
	class IRhiPipelineLayout;

	struct RhiComputePipelineDesc
	{
		IRhiPipelineLayout* _pipelineLayout  = nullptr;
		const void*         _computeShader   = nullptr;
		size_t              _computeShaderSize = 0;
	};

	class IRhiComputePipeline
	{
	public:
		IRhiComputePipeline(const RhiComputePipelineDesc& desc);
		virtual ~IRhiComputePipeline() = default;

	public:
		inline const RhiComputePipelineDesc& getDesc() const { return _desc; }
		inline IRhiPipelineLayout* getPipelineLayout() const { return _desc._pipelineLayout; }

	protected:
		RhiComputePipelineDesc _desc;
	};
}
