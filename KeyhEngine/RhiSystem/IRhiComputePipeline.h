#pragma once
#include "IRhiGraphicsPipeline.h"

namespace keyh
{
	class IRhiPipelineLayout;

	struct RhiComputePipelineDesc
	{
		IRhiPipelineLayout* _pipelineLayout = nullptr;
		RhiShaderBytecode   _computeShader;
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
