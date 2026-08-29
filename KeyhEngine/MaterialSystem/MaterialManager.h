#pragma once
namespace keyh
{
	class MaterialManager
	{
	public:
		MaterialManager();
		~MaterialManager();

	private:
		Vector<StaticStringA> _materialPaths;
	
	};
}

