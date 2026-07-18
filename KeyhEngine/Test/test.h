#pragma once
#include "TestCommon.h"

namespace keyh
{
    class REFLECTIVE(TestSubObject)
    {
        KEYH_REFLECT_BODY(TestSubObject)
    protected:
        KEYH_REFLECT_PROPERTY(PropertyName = "Names")
            Vector<StaticStringA> _names;
    };

    class REFLECTIVE(TestObject)
    {
        KEYH_REFLECT_BODY(TestObject)
    protected:
        KEYH_REFLECT_PROPERTY_GROUP("Group1")
        KEYH_REFLECT_PROPERTY(Default = 0)
        int _intValue = 0;
        KEYH_REFLECT_PROPERTY(PropertyName = "FloatValue", Default = 0.0f)
        float _floatValue = 0.0f;
        KEYH_REFLECT_PROPERTY_GROUP_END

        KEYH_REFLECT_PROPERTY()
        StaticStringA _stringValue;

        KEYH_REFLECT_PROPERTY(PropertyName="Names")
		Vector<StaticStringA> _names;

		KEYH_REFLECT_PROPERTY(PropertyName = "SubObject")
		TestSubObject _subObject;
    };
}
#include "reflect_generated.inl"
