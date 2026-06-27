#pragma once
#include <cstring>
#include "AssertUtil.h"

namespace keyh
{
	template<typename T, typename Derived>
	class IBuffer
	{
	public:
		virtual ~IBuffer() = default;

	protected:
		size_t _offset = 0;

	public:
		inline T*		getBuffer() { return getDerived()->getBufferImpl(); }
		inline const T* getBuffer() const { return getDerived()->getBufferImpl(); }
		inline size_t	size() const { return _offset * sizeof(T); }
		inline size_t	capacity() const { return getDerived()->getCapacityImpl(); }

	private:
		Derived* getDerived() { return static_cast<Derived*>(this); }
		const Derived* getDerived() const { return static_cast<const Derived*>(this); }

	public:
		void	write(const void* input, size_t size);
		void	writeOne(T input);
		void	reset();
		size_t	getAvailableSize() const;
	};
}
#include "IBuffer.hpp"