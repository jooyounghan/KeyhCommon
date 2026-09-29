namespace keyh
{
	template<typename BaseType>
	Vector<typename ReflectTypeRegistry<BaseType>::Entry>& ReflectTypeRegistry<BaseType>::entries()
	{
		static Vector<Entry> result;
		return result;
	}

	template<typename BaseType>
	template<typename DerivedType>
	bool ReflectTypeRegistry<BaseType>::registerType(const StringViewA& name)
	{
		TypeTrait::requireDerivedFrom<BaseType, IReflectObject>();
		TypeTrait::requireDerivedFrom<DerivedType, BaseType>();
		// Restrict names to stable, unescaped identifiers in both wire formats.
		if (name.size() == 0)
			return false;
		for (size_t i = 0; i < name.size(); ++i)
		{
			if (!StrUtil::isValidIdentifierCharacter(name.c_str()[i]))
				return false;
		}
		DerivedType sample;
		const ReflectMetaObject* metaObject = &sample.getMetaObject();
		for (const Entry& entry : entries())
		{
			if (entry.name.getStringView() == name || entry.metaObject == metaObject)
				return entry.name.getStringView() == name && entry.metaObject == metaObject;
		}
		entries().push_back(Entry{ FlyweightStringA(name), metaObject,
			[]() -> Ptr<BaseType> { return makePtr<BaseType, DerivedType>(); } });
		return true;
	}

	template<typename BaseType>
	StringViewA ReflectTypeRegistry<BaseType>::findName(const BaseType& object)
	{
		for (const Entry& entry : entries())
		{
			if (entry.metaObject == &object.getMetaObject())
				return StringViewA(entry.name.c_str(), entry.name.size());
		}
		return StringViewA();
	}

	template<typename BaseType>
	Ptr<BaseType> ReflectTypeRegistry<BaseType>::create(const StringViewA& name)
	{
		for (const Entry& entry : entries())
		{
			if (entry.name.getStringView() == name)
				return entry.create();
		}
		return nullptr;
	}

	template<typename BaseType>
	Ptr<BaseType> ReflectTypeRegistry<BaseType>::createLegacy()
	{
		if constexpr (requires { new BaseType(); })
			return makePtr<BaseType>();
		else
			return nullptr;
	}
}
