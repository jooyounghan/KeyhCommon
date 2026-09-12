#pragma once
namespace keyh
{
	class MaterialLayout;

	class MaterialManager
	{
	public:
		MaterialManager();
		~MaterialManager();

	public:
		using MaterialLayoutMap = HashMap<FlyweightStringA, Ptr<MaterialLayout>>;

	private:
		MaterialLayoutMap _materialLayouts;

	private:
		Vector<StaticStringA> _materialDirectoryPaths;	
	};
}

