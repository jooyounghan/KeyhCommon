#include "ReflectSystemPch.h"
#include "ReflectSerializer.h"
#include "JsonDocument.h"

namespace keyh
{
    void ReflectSerializer::serializeToJson(const StringViewA& filePath, const IReflectObject* reflectObject)
    {
		StaticBuffer<utf8, kBuffer4KBytes> buffer;

		if (reflectObject == nullptr)
		{
			KEYH_ASSERT_ARGS(false, "Reflect object is null.");
			return;
		}

		buffer.writeBytes("{", 1);

		const ReflectMetaObject& metaObject = reflectObject->getMetaObject();
        const OwnerVector<IReflectProperty>& properties = metaObject.getReflectProperties();
		for (const IReflectProperty* reflectProperty : properties)
		{
			if (reflectProperty == nullptr)
				continue;

			const FlyweightStringA& propertyName = reflectProperty->getPropertyName();
			buffer.write(propertyName.c_str(), propertyName.size());
			reflectProperty->serializeToJson(&buffer, reflectObject);
		}

		buffer.writeBytes("}", 1);
    }

    void ReflectSerializer::deserializeFromJson(const StringViewA& filePath, IReflectObject* reflectObject)
    {
        JsonDocument jsonDocument;
        jsonDocument.loadFromFile(filePath.c_str());
        JsonObject rootObject = jsonDocument.getRootObject();

        if (rootObject.isValid() == false)
        {
            KEYH_ASSERT_ARGS(false, "Failed to load JSON document from file: %s", filePath.c_str());
            return;
        }

		for (JsonKey jsonKey = rootObject.getFirstKey(); jsonKey.isValid(); jsonKey = rootObject.getNextKey(jsonKey))
		{
			const StringViewA keyName = jsonKey.getKeyName();
			const IReflectProperty* property = reflectObject->getMetaObject().findProperty(keyName);
			if (property == nullptr)
				continue;

			property->deserializeFromJson(jsonKey.getValue(), reflectObject);
		}
    }
}
