#pragma once
#include <cstring>
#include "AssertUtil.h"

namespace keyh
{
	class IBufferBase
	{
	public:
		virtual ~IBufferBase() = default;

	public:
		virtual void		writeBytes(const void* input, size_t size) = 0;
		virtual void		resetRaw() = 0;
		virtual size_t		getSizeBytes() const = 0;
		virtual size_t		getCapacityBytes() const = 0;
		virtual void*		getRawBuffer() = 0;
		virtual const void*	getRawBuffer() const = 0;
		virtual size_t		getAvailableSizeBytes() const = 0;
	};

	template<typename T, typename Derived>
	class IBuffer : public IBufferBase
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

	public:
		void		writeBytes(const void* input, size_t size) override { write(input, size); }
		void		resetRaw() override { reset(); }
		size_t		getSizeBytes() const override { return size(); }
		size_t		getCapacityBytes() const override { return capacity(); }
		void*		getRawBuffer() override { return getBuffer(); }
		const void*	getRawBuffer() const override { return getBuffer(); }
		size_t		getAvailableSizeBytes() const override { return getAvailableSize(); }
	};
}
#include "IBuffer.hpp"