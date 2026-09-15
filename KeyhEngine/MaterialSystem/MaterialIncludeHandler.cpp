#include "MaterialSystemPch.h"
#include "MaterialIncludeHandler.h"
#include "ShaderCompiler.h"

namespace keyh
{
	MaterialIncludeHandler::MaterialIncludeHandler(const FlyweightStringA& materialGroupName, const FlyweightStringA& materialName)
	{
	
	}

	HRESULT __stdcall MaterialIncludeHandler::LoadSource(
		_In_ LPCWSTR pFilename
		, _COM_Outptr_result_maybenull_ IDxcBlob** ppIncludeSource
	)
	{
		inline constexpr const wchar_t kVirtualMaterialPath[] = L".\\VirtualMaterial.hlsl";
		inline constexpr size_t kVirtualMaterialPathCount = sizeof(kVirtualMaterialPath) / sizeof(wchar_t) - 1;

		StaticStringW fileNameW = StaticStringW(pFilename);
		StringViewW fileNameView(fileNameW.c_str(), fileNameW.length());

		if (fileNameView == StringViewW(kVirtualMaterialPath, kVirtualMaterialPathCount))
		{
			*ppIncludeSource = _currentMaterialBlob.Get();
			_currentMaterialBlob->AddRef();
			return S_OK;
		}
		else
		{	
			IDxcIncludeHandler* defaultIncludeHandler = ShaderCompiler::getDefaultIncludeHandler();
			if (defaultIncludeHandler == nullptr)
			{
				KEYH_ASSERT_DEV(false, "Default include handler is not initialized.");
				return E_FAIL;
			}

			return defaultIncludeHandler->LoadSource(pFilename, ppIncludeSource);
		}
		return E_NOTIMPL;
	}
}
