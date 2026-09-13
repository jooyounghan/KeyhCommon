#pragma once
namespace keyh
{
	class MaterialLayoutRegistry;
	class MaterialLayout;

	class MaterialManager
	{
	public:
		MaterialManager();
		~MaterialManager();
		
	private:
		bool _forceRebuildMaterialLayout = false;

	public:
		using MaterialLayoutMap = HashMap<FlyweightStringA, Ptr<MaterialLayout>>;

	private:
		Ptr<MaterialLayoutRegistry>		_materialLayoutRegistry;
		MaterialLayoutMap				_materialLayouts;

	private:
		Vector<StaticStringA> _materialDirectoryPaths;	

	private:
		void importMaterialLayout(const Vector<StaticStringA>& directoryPaths);
#if defined KEYH_DEV
		void importMaterialDefinitions(const Vector<StaticStringA>& directoryPaths);
#endif
	};
}

