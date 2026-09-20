#pragma once
namespace keyh
{
	class MaterialLayout;

	using MaterialGroupTable = HashMap<FlyweightStringA, Vector<FlyweightStringA>>;
	class REFLECTIVE(MaterialLayoutRegistry)
	{
		KEYH_REFLECT_DECLARE_BODY(MaterialLayoutRegistry)
			
	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialGroupTables")
		MaterialGroupTable _materialGroupTables;

	public:
		inline MaterialGroupTable& getMaterialGroupTables() { return _materialGroupTables; }

	public:
		void mergeMaterialGroupTables(const MaterialGroupTable& otherMaterialGroupTables);
	};
}

#include "MaterialLayoutRegistry.reflect_generated.inl"
