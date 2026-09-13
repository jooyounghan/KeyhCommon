#include "MaterialSystemPch.h"
#include "MaterialLayoutRegistry.h"

namespace keyh
{
	KEYH_REFLECT_DEFINE_BODY(MaterialLayoutRegistry);

	void MaterialLayoutRegistry::mergeMaterialGroupTables(const MaterialGroupTable& otherMaterialGroupTables)
	{
		for (const HashBucket<FlyweightStringA, Vector<FlyweightStringA>>& bucket : otherMaterialGroupTables)
		{
			const FlyweightStringA& groupName = bucket.key();
			const Vector<FlyweightStringA>& materialNames = bucket.value();

			MaterialGroupTable::FindResult findResult = _materialGroupTables.find(groupName);
			if (findResult.isFound() == true)
			{
				Vector<FlyweightStringA>& existingMaterialNames = *findResult.value();
				for (const FlyweightStringA& materialName : materialNames)
				{
					bool isDuplicate = false;
					for (const FlyweightStringA& existingMaterialName : existingMaterialNames)
					{
						if (existingMaterialName == materialName)
						{
							isDuplicate = true;
							break;
						}
					}

					if (isDuplicate == false)
						existingMaterialNames.push_back(materialName);
				}
			}
			else
			{
				_materialGroupTables.insert(groupName, materialNames);
			}
		}
	}
}