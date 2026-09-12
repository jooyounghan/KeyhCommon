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

	private:
		void importMaterialLayout(const Vector<StaticStringA>& directoryPaths);
#if defined KEYH_DEV
		void importMaterialDefinitions(const Vector<StaticStringA>& directoryPaths);
#endif
	};
}

