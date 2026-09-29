#pragma once
#include "Delegate.h"
#include "OwnerVector.h"

namespace keyh
{
	template <typename Signature>
	class MulticastDelegate;

#define DECLARE_MULTICAST_DELEGATE(DelegateName, ReturnType, ...) \
	using DelegateName = MulticastDelegate<ReturnType(__VA_ARGS__)>;

	template <typename ReturnType, typename... Args>
	class MulticastDelegate<ReturnType(Args...)>
	{
		KEYH_STATIC_ASSERT((IsSame_v<ReturnType, void>), "MulticastDelegate callbacks must return void.");
		KEYH_STATIC_ASSERT(((IsLvalueReference_v<Args> || IsSame_v<Args, RemoveReference_t<Args>>) && ...),
			"MulticastDelegate does not support rvalue-reference parameters.");

	public:
		using Handle = size_t;
		static constexpr Handle kInvalidHandle = static_cast<Handle>(-1);

	private:
		using DelegateType = Delegate<ReturnType(Args...)>;

		struct Entry
		{
			Handle _handle;
			DelegateType _delegate;

			explicit Entry(Handle handle) : _handle(handle) {}
		};

		class InvokeScope
		{
		private:
			size_t& _depth;

		public:
			explicit InvokeScope(size_t& depth) : _depth(depth) { ++_depth; }
			~InvokeScope() { --_depth; }
		};

		OwnerVector<Entry> _delegates;
		mutable size_t _invokeDepth = 0;
		Handle _nextHandle = 0;

	private:
		bool containsHandle(Handle handle) const;
		Handle allocateHandle();

	public:
		MulticastDelegate() = default;
		~MulticastDelegate() = default;

		REMOVE_COPY(MulticastDelegate);

		MulticastDelegate(MulticastDelegate&& other) noexcept = default;
		MulticastDelegate& operator=(MulticastDelegate&& other) noexcept = default;

	public:
		template <typename TargetClass>
		Handle bind(TargetClass* instance, ReturnType(TargetClass::* method)(Args...));

		template <typename F>
		Handle bind(F&& callable);

		bool unbind(Handle handle);
		void clear();

	public:
		void invoke(Args... args) const;
		inline void broadcast(Args... args) const { invoke(args...); }
		inline void operator()(Args... args) const { invoke(args...); }

	public:
		inline size_t size() const { return _delegates.size(); }
		inline bool empty() const { return _delegates.size() == 0; }
	};
}

#include "MulticastDelegate.hpp"
