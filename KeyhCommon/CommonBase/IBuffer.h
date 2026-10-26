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

	public:
		virtual void*		getRawBuffer() = 0;
		virtual const void*	getRawBuffer() const = 0;

	public:
		virtual size_t		size() const = 0;
		virtual size_t		capacity() const = 0;
		virtual size_t		getAvailableSize() const = 0;
	};

	template<typename T, typename Derived>
	class IBufferBase : public IBuffer
	{
	public:
		virtual ~IBufferBase() = default;

	protected:
		size_t _offset = 0;

	public:
		virtual void			writeBytes(const void* input, size_t size) override;

	public:
		inline virtual void*		getRawBuffer() override { return getBuffer(); }
		inline virtual const void*	getRawBuffer() const override { return getBuffer(); }

	public:
		inline virtual size_t	size() const override { return _offset; }
		inline virtual size_t	capacity() const override { return getDerived()->getCapacityImpl(); }
		virtual size_t			getAvailableSize() const override;

	private:
		Derived*		getDerived() { return static_cast<Derived*>(this); }
		const Derived*	getDerived() const { return static_cast<const Derived*>(this); }

	public:
		inline T*		getBuffer() { return getDerived()->getBufferImpl(); }
		inline const T* getBuffer() const { return getDerived()->getBufferImpl(); }

	public:
		void	write(const void* input, size_t count);
		void	writeOne(T input);
		void	reset();
	};
}
#include "IBuffer.hpp"
