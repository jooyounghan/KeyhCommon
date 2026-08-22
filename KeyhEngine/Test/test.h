#pragma once
#include "TestCommon.h"

namespace keyh
{
    // Deepest level: holds a list of tags and a single integer counter.
    class REFLECTIVE(TestDeepObject)
    {
        KEYH_REFLECT_BODY(TestDeepObject)
    public:
        KEYH_REFLECT_PROPERTY(PropertyName = "Tags")
        Vector<StaticStringA> _tags;

        KEYH_REFLECT_PROPERTY(PropertyName = "Counter", Default = 0)
        int _counter = 0;
    };

    // Mid level: holds a list of labels, a nested TestDeepObject, and a float score.
    class REFLECTIVE(TestNestedObject)
    {
        KEYH_REFLECT_BODY(TestNestedObject)
    public:
        KEYH_REFLECT_PROPERTY(PropertyName = "Labels")
        Vector<StaticStringA> _labels;

        KEYH_REFLECT_PROPERTY(PropertyName = "Deep")
        TestDeepObject _deep;

        KEYH_REFLECT_PROPERTY(PropertyName = "Score", Default = 0.0f)
        float _score = 0.0f;
    };

    class REFLECTIVE(TestSubObject)
    {
        KEYH_REFLECT_BODY(TestSubObject)
    public:
        KEYH_REFLECT_PROPERTY(PropertyName = "Names")
            Vector<StaticStringA> _names;
    };

    class REFLECTIVE(TestObject)
    {
        KEYH_REFLECT_BODY(TestObject)
    public:
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

		KEYH_REFLECT_PROPERTY(PropertyName = "Nested")
		TestNestedObject _nested;
    };
}
#include "reflect_generated.inl"
