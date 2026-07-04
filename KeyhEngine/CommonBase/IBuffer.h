#pragma once
#include <cstring>
#include "AssertUtil.h"

namespace keyh
{
	class IBuffer
	{
	public:
		virtual ~IBuffer() = default;

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
	class IBufferBase : public IBuffer
	{
	public:
		virtual ~IBufferBase() = default;

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
		inline virtual void			writeBytes(const void* input, size_t size) override { write(input, size); }
		inline virtual void			resetRaw() override { reset(); }
		inline virtual size_t		getSizeBytes() const override { return size(); }
		inline virtual size_t		getCapacityBytes() const override { return capacity(); }
		inline virtual void*		getRawBuffer() override { return getBuffer(); }
		inline virtual const void*	getRawBuffer() const override { return getBuffer(); }
		inline virtual size_t		getAvailableSizeBytes() const override { return getAvailableSize(); }
	};
}
#include "IBuffer.hpp"