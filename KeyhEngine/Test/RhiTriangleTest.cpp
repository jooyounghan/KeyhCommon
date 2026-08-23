#include "TestCommon.h"
#include "RhiTriangleTest.h"

using namespace keyh;

#if defined(KEYH_PLATFORM_WINDOWS)
#include <cstring>
#include <dxcapi.h>
#endif

namespace
{
#if defined(KEYH_PLATFORM_WINDOWS)
	const char* kTriangleShaderSource = R"(
struct VSOutput
{
    float4 Position : SV_Position;
    float4 Color    : COLOR0;
};

VSOutput VSMain(uint vertexId : SV_VertexID)
{
    const float2 positions[3] =
    {
        float2( 0.0f,  0.5f),
        float2( 0.5f, -0.5f),
        float2(-0.5f, -0.5f)
    };

    const float4 colors[3] =
    {
        float4(1.0f, 0.0f, 0.0f, 1.0f),
        float4(0.0f, 1.0f, 0.0f, 1.0f),
        float4(0.0f, 0.0f, 1.0f, 1.0f)
    };

    VSOutput output;
    output.Position = float4(positions[vertexId], 0.0f, 1.0f);
    output.Color = colors[vertexId];
    return output;
}

float4 PSMain(VSOutput input) : SV_Target0
{
    return input.Color;
}
)";

	bool compileHlslToDxil(const wchar_t* entryPoint, const wchar_t* targetProfile, Microsoft::WRL::ComPtr<IDxcBlob>& outBlob)
	{
		Microsoft::WRL::ComPtr<IDxcCompiler3> compiler;
		if (FAILED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler))))
		{
			return false;
		}

		DxcBuffer sourceBuffer = {};
		sourceBuffer.Ptr = kTriangleShaderSource;
		sourceBuffer.Size = std::strlen(kTriangleShaderSource);
		sourceBuffer.Encoding = DXC_CP_UTF8;

		const wchar_t* arguments[] =
		{
			L"-E", entryPoint,
			L"-T", targetProfile,
			L"-Zpr"
		};

		Microsoft::WRL::ComPtr<IDxcResult> result;
		const HRESULT hr = compiler->Compile(
			&sourceBuffer,
			arguments,
			static_cast<uint32>(_countof(arguments)),
			nullptr,
			IID_PPV_ARGS(&result)
		);

		if (FAILED(hr) || result == nullptr)
		{
			return false;
		}

		HRESULT compileStatus = S_OK;
		result->GetStatus(&compileStatus);
		if (FAILED(compileStatus))
		{
			Microsoft::WRL::ComPtr<IDxcBlobUtf8> errors;
			if (SUCCEEDED(result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr)) && errors != nullptr && errors->GetStringLength() > 0)
			{
				std::printf("[DXC] %s\n", errors->GetStringPointer());
			}
			return false;
		}

		Microsoft::WRL::ComPtr<IDxcBlob> shaderBlob;
		if (FAILED(result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr)) || shaderBlob == nullptr)
		{
			return false;
		}

		outBlob = keyh::move(shaderBlob);
		return true;
	}
#endif
}

void test_RhiSystem_d3d12_triangle_example()
{
	printSection("RhiSystem - D3D12 triangle draw example");

#if !defined(KEYH_PLATFORM_WINDOWS)
	std::printf("  [SKIP] D3D12 example is only available on Windows.\n");
	return;
#else
	Ptr<IRhiInstance> instance = IRhiInstance::create();
	CHECK(instance != nullptr);
	if (instance == nullptr)
	{
		return;
	}

	const bool instanceInitialized = instance->initialize();
	CHECK(instanceInitialized);
	if (!instanceInitialized)
	{
		return;
	}

	OwnerVector<IRhiDevice>& devices = instance->getDevices();
	CHECK(devices.size() > 0);
	if (devices.size() == 0)
	{
		return;
	}

	IRhiDevice* device = devices[0];
	CHECK(device != nullptr);
	if (device == nullptr)
	{
		return;
	}

	Ptr<IRhiCommandPool> commandPool = device->createCommandPool(ECommandQueueType::Direct);
	CHECK(commandPool != nullptr);
	if (commandPool == nullptr)
	{
		return;
	}

	Ptr<IRhiCommandList> commandList = commandPool->allocateCommandList();
	CHECK(commandList != nullptr);
	if (commandList == nullptr)
	{
		return;
	}

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShader;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShader;
	const bool vertexCompileSucceeded = compileHlslToDxil(L"VSMain", L"vs_6_0", vertexShader);
	const bool pixelCompileSucceeded = compileHlslToDxil(L"PSMain", L"ps_6_0", pixelShader);
	CHECK(vertexCompileSucceeded);
	CHECK(pixelCompileSucceeded);

	if (vertexCompileSucceeded && pixelCompileSucceeded)
	{
		EResourceFormat renderTargetFormat = EResourceFormat::R8G8B8A8_UNorm;
		RhiGraphicsPipelineDesc pipelineDesc = {};
		pipelineDesc._vertexShader._data = vertexShader->GetBufferPointer();
		pipelineDesc._vertexShader._sizeInBytes = vertexShader->GetBufferSize();
		pipelineDesc._pixelShader._data = pixelShader->GetBufferPointer();
		pipelineDesc._pixelShader._sizeInBytes = pixelShader->GetBufferSize();
		pipelineDesc._primitiveTopologyType = EPrimitiveTopologyType::Triangle;
		pipelineDesc._renderTargetFormats = &renderTargetFormat;
		pipelineDesc._renderTargetCount = 1;
		pipelineDesc._sampleCount = 1;

		Ptr<IRhiGraphicsPipeline> graphicsPipeline = device->createGraphicsPipeline(pipelineDesc);
		CHECK(graphicsPipeline != nullptr);

		if (graphicsPipeline != nullptr)
		{
			commandList->begin();
			commandList->setGraphicsPipeline(graphicsPipeline.get());
			commandList->setPrimitiveTopology(EPrimitiveTopologyType::Triangle);
			commandList->drawInstanced(3, 1, 0, 0);
			commandList->end();
		}
	}
#endif
}
