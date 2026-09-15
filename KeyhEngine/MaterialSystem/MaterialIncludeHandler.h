#pragma once

namespace keyh
{
#ifdef KEYH_PLATFORM_WINDOWS

	using DxcIncludeHandlerBase = Microsoft::WRL::RuntimeClass<
		Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
		IDxcIncludeHandler
	>;

	class MaterialIncludeHandler : public DxcIncludeHandlerBase
	{
	public:
		MaterialIncludeHandler() = delete;
		explicit MaterialIncludeHandler(const FlyweightStringA& materialGroupName, const FlyweightStringA& materialName);

	public:
		HRESULT STDMETHODCALLTYPE LoadSource(
			_In_ LPCWSTR pFilename,
			_COM_Outptr_result_maybenull_ IDxcBlob** ppIncludeSource
		) override;

	private:
		Microsoft::WRL::ComPtr<IDxcBlob> _currentMaterialBlob = nullptr;
	};
#endif
}

