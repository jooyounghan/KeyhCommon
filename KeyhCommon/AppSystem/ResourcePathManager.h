#pragma once

namespace keyh
{
	KEYH_REFLECT_ENUM
	enum class EProjectPath : uint8
	{
		Common, Main, Additional0, Additional1, Additional2, Additional3, Additional4, Additional5, Count
	};

	class REFLECTIVE(ProjectNode)
	{
		KEYH_REFLECT_DECLARE_BODY(ProjectNode)
	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "ProjectPath")
		EProjectPath _projectPath = EProjectPath::Common;

		KEYH_REFLECT_PROPERTY(PropertyName = "Description")
		StaticStringA _description;

		KEYH_REFLECT_PROPERTY(PropertyName = "Children")
		Vector<ProjectNode> _children;

	public:
		inline EProjectPath					getProjectPath() const { return _projectPath; }
		inline const Vector<ProjectNode>&	getChildren() const { return _children; }
	};

	class ResourcePathManager
	{
		SINGLETON(ResourcePathManager);

	public:
		void initializeResourcePaths();

	private:
		static constexpr uint8 kProjectPathCount = static_cast<uint8>(EProjectPath::Count);
		StaticStringA _projectPaths[kProjectPathCount];
		Vector<StringViewA> _orderedResourcePaths;
		ProjectNode _rootNode;

	private:
		bool appendPathsInReverseTopologicalOrder(const ProjectNode& projectNode);

	public:
		const Vector<StringViewA>& getOrderedResourcePaths() const { return _orderedResourcePaths; }
		const StaticStringA& getResourcePath(EProjectPath projectPath) const { return _projectPaths[static_cast<uint8>(projectPath)]; }
	};
}

#include "generated/ResourcePathManager.reflect_generated.inl"

