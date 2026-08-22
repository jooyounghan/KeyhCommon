#pragma once
#include "RhiEnum.h"

namespace keyh
{
	class IRhiPipelineLayout;

	struct RhiShaderBytecode
	{
		const void* _data = nullptr;
		size_t _sizeInBytes = 0;
	};

	struct RhiGraphicsPipelineDesc
	{
		IRhiPipelineLayout* _pipelineLayout = nullptr;
		RhiShaderBytecode _vertexShader;
		RhiShaderBytecode _pixelShader;
		const EResourceFormat* _renderTargetFormats = nullptr;
		uint32 _renderTargetCount = 0;
		EResourceFormat _depthStencilFormat = EResourceFormat::Unknown;
		uint32 _sampleCount = 1;
		bool _enableDepthTest = false;
		bool _enableDepthWrite = false;
		EComparisonFunction _depthComparisonFunc = EComparisonFunction::LessEqual;
	};

	class IRhiGraphicsPipeline
	{
	public:
		IRhiGraphicsPipeline(const RhiGraphicsPipelineDesc& desc);
		virtual ~IRhiGraphicsPipeline() = default;

	public:
		inline const RhiGraphicsPipelineDesc& getDesc() const { return _desc; }
		inline IRhiPipelineLayout* getPipelineLayout() const { return _desc._pipelineLayout; }

	protected:
		RhiGraphicsPipelineDesc _desc;
	};
}
