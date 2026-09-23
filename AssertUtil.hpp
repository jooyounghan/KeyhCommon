namespace keyh
{
	template<typename ...Args>
	void AssertUtil::check(bool condition, const char* format, Args && ...args) noexcept
	{
		if (condition == true)
			return;

		char messageBuffer[2048];
		int result = snprintf(messageBuffer, sizeof(messageBuffer), format, keyh::forward<Args>(args)...);

		if (result < 0)
		{
			messageBuffer[0] = '\0';
		}

		if (gAssertHandler.isValid() && gAssertHandler(messageBuffer))
		{
			KEYH_DEBUG_BREAK();
		}
		else
		{
			std::abort();
		}
	}
}