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

	struct RhiRasterizerDesc
	{
		EFillMode _fillMode                     = EFillMode::Solid;
		ECullMode _cullMode                     = ECullMode::Back;
		bool      _frontCounterClockwise        = false;
		int32     _depthBias                    = 0;
		float     _depthBiasClamp               = 0.f;
		float     _slopeScaledDepthBias         = 0.f;
		bool      _depthClipEnable              = true;
		bool      _multisampleEnable            = false;
		bool      _antialiasedLineEnable        = false;
		uint32    _forcedSampleCount            = 0;
		bool      _conservativeRasterEnable     = false;
	};

	struct RhiRenderTargetBlendDesc
	{
		bool          _blendEnable           = false;
		bool          _logicOpEnable         = false;
		EBlendFactor  _srcBlend              = EBlendFactor::One;
		EBlendFactor  _destBlend             = EBlendFactor::Zero;
		EBlendOp      _blendOp               = EBlendOp::Add;
		EBlendFactor  _srcBlendAlpha         = EBlendFactor::One;
		EBlendFactor  _destBlendAlpha        = EBlendFactor::Zero;
		EBlendOp      _blendOpAlpha          = EBlendOp::Add;
		ELogicOp      _logicOp               = ELogicOp::Noop;
		EColorWriteMask _renderTargetWriteMask = EColorWriteMask::All;
	};

	static constexpr uint32 RhiMaxRenderTargets = 8;

	struct RhiBlendDesc
	{
		bool                   _alphaToCoverageEnable  = false;
		bool                   _independentBlendEnable = false;
		RhiRenderTargetBlendDesc _renderTarget[RhiMaxRenderTargets] = {};
	};

	struct RhiDepthStencilOpDesc
	{
		EStencilOp         _stencilFailOp      = EStencilOp::Keep;
		EStencilOp         _stencilDepthFailOp = EStencilOp::Keep;
		EStencilOp         _stencilPassOp      = EStencilOp::Keep;
		EComparisonFunction _stencilFunc       = EComparisonFunction::Always;
	};

	struct RhiDepthStencilDesc
	{
		bool                _depthEnable      = false;
		bool                _depthWriteEnable = false;
		EComparisonFunction _depthFunc        = EComparisonFunction::LessEqual;
		bool                _stencilEnable    = false;
		uint8               _stencilReadMask  = 0xFF;
		uint8               _stencilWriteMask = 0xFF;
		RhiDepthStencilOpDesc _frontFace      = {};
		RhiDepthStencilOpDesc _backFace       = {};
	};

	struct RhiGraphicsPipelineDesc
	{
		IRhiPipelineLayout*       _pipelineLayout      = nullptr;
		RhiShaderBytecode         _vertexShader;
		RhiShaderBytecode         _pixelShader;
		RhiShaderBytecode         _hullShader;
		RhiShaderBytecode         _domainShader;
		RhiShaderBytecode         _geometryShader;
		RhiShaderBytecode         _amplificationShader;
		RhiShaderBytecode         _meshShader;
		RhiRasterizerDesc         _rasterizerDesc;
		RhiBlendDesc              _blendDesc;
		RhiDepthStencilDesc       _depthStencilDesc;
		EPrimitiveTopologyType    _primitiveTopologyType = EPrimitiveTopologyType::Triangle;
		const EResourceFormat*    _renderTargetFormats   = nullptr;
		uint32                    _renderTargetCount     = 0;
		EResourceFormat           _depthStencilFormat    = EResourceFormat::Unknown;
		uint32                    _sampleCount           = 1;
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

