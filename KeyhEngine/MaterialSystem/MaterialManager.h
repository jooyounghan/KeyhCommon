#pragma once
namespace keyh
{
	class MaterialInfo;

	class MaterialManager
	{
	public:
		MaterialManager();
		~MaterialManager();

	private:
		Vector<StaticStringA> _materialDirectoryPaths;
	

	public:
		using MaterialInfoMap = HashMap<FlyweightStringA, Ptr<MaterialInfo>>;

	private:
		MaterialInfoMap _materialInfos;
	};
}

