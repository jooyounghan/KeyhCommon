namespace keyh
{
	template<typename Predicate>
	void ConditionalVariable::wait(Mutex& mutex, Predicate&& predicate)
	{
		while (!predicate())
			wait(mutex);
	}
}
