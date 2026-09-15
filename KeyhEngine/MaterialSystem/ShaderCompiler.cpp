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
		constexpr size_t shaderProfileLength = sizeof(shaderProfiles[0]) / sizeof(wchar_t) - 1;

        const size_t shaderTypeIndex = static_cast<size_t>(shaderStage);
		StaticBuffer<wchar_t, 16> targetProfileBuffer;
		targetProfileBuffer.write(shaderProfiles[shaderTypeIndex], shaderProfileLength);
        targetProfileBuffer.write(shaderCompileVersion.c_str(), shaderCompileVersion.size());
        std::wstring targetProfile = shaderProfiles[shaderTypeIndex] + std::wstring(shaderCompileVersion.c_str(), shaderCompileVersion.c_str() + shaderCompileVersion.length());
        return targetProfile;
    }

}