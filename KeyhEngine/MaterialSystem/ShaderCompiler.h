namespace keyh
{
	// Shader Compiler는 DXC Compiler + HLSL를 기반으로 하기에 
	// Window를 위한 클래스로 설계
	
#ifdef KEYH_PLATFORM_WINDOWS
	struct ShaderBinaryData
	{
		Microsoft::WRL::ComPtr<IDxcBlob> _shaderCacheBlob;
		Microsoft::WRL::ComPtr<IDxcBlob> _rootSignatureCacheBlob;
	};

	class ShaderCompiler
	{
	public:
		ShaderCompiler() = default;
		~ShaderCompiler() = default;

	protected:
		Microsoft::WRL::ComPtr<IDxcCompiler3>   _compiler;
		Microsoft::WRL::ComPtr<IDxcUtils>       _utils;

	protected:
		static Microsoft::WRL::ComPtr<IDxcIncludeHandler> _defaultIncludeHandler;

	public:
		static IDxcIncludeHandler* getDefaultIncludeHandler() { return _defaultIncludeHandler.Get(); }

	public:
		void				initialize();
		ShaderBinaryData	compileShader(
			const FlyweightStringW& shaderSource
			, EShaderStage shaderStage
			, const FlyweightStringW& entryPoint
			, const FlyweightStringW& shaderCompileVersion
#ifdef KEYH_DEV	
			, bool isDevShader = false
#endif
		);
	};
#endif
}