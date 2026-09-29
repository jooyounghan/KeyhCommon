#pragma once

namespace keyh
{
	template <typename ReturnType, typename... Args>
	bool MulticastDelegate<ReturnType(Args...)>::containsHandle(Handle handle) const
	{
		for (size_t index = 0; index < _delegates.size(); ++index)
		{
			if (_delegates[index]->_handle == handle)
			{
				return true;
			}
		}
		return false;
	}

	template <typename ReturnType, typename... Args>
	typename MulticastDelegate<ReturnType(Args...)>::Handle
	MulticastDelegate<ReturnType(Args...)>::allocateHandle()
	{
		Handle handle = _nextHandle;
		while (handle == kInvalidHandle || containsHandle(handle))
		{
			++handle;
			if (handle == kInvalidHandle)
			{
				handle = 0;
			}
		}

		_nextHandle = handle + 1;
		if (_nextHandle == kInvalidHandle)
		{
			_nextHandle = 0;
		}
		return handle;
	}

	template <typename ReturnType, typename... Args>
	template <typename TargetClass>
	typename MulticastDelegate<ReturnType(Args...)>::Handle
	MulticastDelegate<ReturnType(Args...)>::bind(
		TargetClass* instance,
		ReturnType(TargetClass::* method)(Args...))
	{
		KEYH_ASSERT(_invokeDepth == 0, "Cannot bind a multicast delegate while it is invoking callbacks.");
		if (_invokeDepth != 0)
		{
			return kInvalidHandle;
		}

		const Handle handle = allocateHandle();
		Entry* entry = _delegates.emplace_back(handle);
		entry->_delegate.bind(instance, method);
		return handle;
	}

	template <typename ReturnType, typename... Args>
	template <typename F>
	typename MulticastDelegate<ReturnType(Args...)>::Handle
	MulticastDelegate<ReturnType(Args...)>::bind(F&& callable)
	{
		KEYH_ASSERT(_invokeDepth == 0, "Cannot bind a multicast delegate while it is invoking callbacks.");
		if (_invokeDepth != 0)
		{
			return kInvalidHandle;
		}

		const Handle handle = allocateHandle();
		Entry* entry = _delegates.emplace_back(handle);
		entry->_delegate.bind(keyh::forward<F>(callable));
		return handle;
	}

	template <typename ReturnType, typename... Args>
	bool MulticastDelegate<ReturnType(Args...)>::unbind(Handle handle)
	{
		KEYH_ASSERT(_invokeDepth == 0, "Cannot unbind a multicast delegate while it is invoking callbacks.");
		if (_invokeDepth != 0)
		{
			return false;
		}

		for (size_t index = 0; index < _delegates.size(); ++index)
		{
			if (_delegates[index]->_handle == handle)
			{
				_delegates.erase(index);
				return true;
			}
		}
		return false;
	}

	template <typename ReturnType, typename... Args>
	void MulticastDelegate<ReturnType(Args...)>::clear()
	{
		KEYH_ASSERT(_invokeDepth == 0, "Cannot clear a multicast delegate while it is invoking callbacks.");
		if (_invokeDepth != 0)
		{
			return;
		}

		_delegates.clear();
	}

	template <typename ReturnType, typename... Args>
	void MulticastDelegate<ReturnType(Args...)>::invoke(Args... args) const
	{
		InvokeScope invokeScope(_invokeDepth);
		for (size_t index = 0; index < _delegates.size(); ++index)
		{
			_delegates[index]->_delegate.invoke(args...);
		}
	}
}
