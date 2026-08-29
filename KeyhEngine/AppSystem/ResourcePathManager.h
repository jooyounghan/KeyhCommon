#pragma once

namespace keyh
{
	class ResourcePathManager
	{
		SINGLETON(ResourcePathManager);

	public:
		void initializeResourcePaths();

	private:
		StaticStringA _commonResourcePath;
		StaticStringA _projectResourcePath;	

	public:
		const StaticStringA& getCommonResourcePath() const { return _commonResourcePath; }
		const StaticStringA& getProjectResourcePath() const { return _projectResourcePath; }
	};
}

