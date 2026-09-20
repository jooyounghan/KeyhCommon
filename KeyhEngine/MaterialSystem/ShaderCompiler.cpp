#include "MaterialSystemPch.h"
#include "ShaderCompiler.h"

namespace keyh
{
	void ShaderCompiler::initialize()
	{
		DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&_compiler));
		DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&_utils));

		if (_defaultIncludeHandler == nullptr)
		{
			_utils->CreateDefaultIncludeHandler(_defaultIncludeHandler.GetAddressOf());
		}
	}

    static StaticStringW getTargetProfile(EShaderStage shaderStage, const FlyweightStringW& shaderCompileVersion)
    {
        constexpr const wchar_t* shaderProfiles[] =
        {
            L"cs_", // Compute Shader
            L"vs_", // Vertex Shader
            L"ps_", // Pixel Shader
            L"hs_", // Hull Shader
            L"ds_", // Domain Shader
            L"gs_", // Geometry Shader
            L"as_", // Amplification Shader
            L"ms_"  // Mesh Shader
        };
		constexpr size_t shaderProfileLength = sizeof(L"xx_") / sizeof(wchar_t) - 1;

        const size_t shaderTypeIndex = static_cast<size_t>(shaderStage);
		StaticBuffer<wchar_t, 16> targetProfileBuffer;
		targetProfileBuffer.write(shaderProfiles[shaderTypeIndex], shaderProfileLength);
        targetProfileBuffer.write(shaderCompileVersion.c_str(), shaderCompileVersion.size());
        
        return StaticStringW(targetProfileBuffer.getBuffer(), targetProfileBuffer.size());
    }

    ShaderBinaryData ShaderCompiler::compileShader(
        const FlyweightStringW& shaderSource
        , EShaderStage shaderStage
        , const FlyweightStringW& entryPoint
        , const FlyweightStringW& shaderCompileVersion
#ifdef KEYH_DEV
		, bool isDevShader
#endif
	)
    {
  //      File file;
  //      if (!file.load(shaderSource.c_str()))
  //          return false;

		//ShaderBinaryData shaderBinaryData;

  //      DxcBuffer sourceBuffer = {};
  //      sourceBuffer.Ptr = shaderCodeBuffer->getConstBuffer();
  //      sourceBuffer.Size = shaderCodeBuffer->getAvailableSize();
  //      sourceBuffer.Encoding = DXC_CP_UTF8;


  //      std::wstring entryPointW = StrUtil::toWString(entryPoint.c_str());
  //      std::wstring targetProfileW = getTargetProfile(shaderType, shaderCompileVersion);

  //      std::vector<LPCWSTR> arguments;

  //      arguments.push_back(L"-I");
  //      arguments.push_back(_shaderPathW.c_str());

  //      arguments.push_back(L"-E");
  //      arguments.push_back(entryPointW.c_str());

  //      arguments.push_back(L"-T");
  //      arguments.push_back(targetProfileW.c_str());

  //      if (isDevShader)
  //      {
  //          arguments.push_back(DXC_ARG_DEBUG);
  //      }
  //      else
  //      {
  //          arguments.push_back(DXC_ARG_OPTIMIZATION_LEVEL3);
  //      }

  //      Microsoft::WRL::ComPtr<IDxcResult> pResult;

  //      HRESULT hr = _compiler->Compile(
  //          &sourceBuffer,
  //          arguments.data(),
  //          static_cast<uint32_t>(arguments.size()),
  //          includeHandler,
  //          IID_PPV_ARGS(&pResult)
  //      );


  //      handleShaderComileResult(hr, pResult.Get());

  //      ShaderBinaryData& shaderBinaryData = PipelineState->_shaderBinaryDataList[static_cast<size_t>(shaderType)];

  //      pResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBinaryData._shaderCacheBlob), nullptr);
  //      pResult->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&shaderBinaryData._rootSignatureCacheBlob), nullptr);

    }

}