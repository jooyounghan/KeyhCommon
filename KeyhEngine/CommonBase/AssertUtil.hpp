namespace keyh
{
	template<typename ...Args>
	void AssertUtil::check(bool condition, const char* format, Args && ...args) noexcept
	{
		if (condition == false)
		{
			char messageBuffer[2048];
			int result = snprintf(messageBuffer, sizeof(messageBuffer), format, std::forward<Args>(args)...);

			if (result < 0)
			{
				messageBuffer[0] = '\0';
			}

			if (gAssertHandler.isValid() && gAssertHandler(messageBuffer))
			{
				__debugbreak();
			}
			else
			{
				std::abort();
			}
		}
	}
}