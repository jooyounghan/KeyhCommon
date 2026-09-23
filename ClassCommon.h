#pragma once

#define REMOVE_COPY(ClassName) \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete;

#define REMOVE_MOVE(ClassName) \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete;

#define REMOVE_COPY_AND_MOVE(ClassName) \
	REMOVE_COPY(ClassName) \
	REMOVE_MOVE(ClassName)

#define SINGLETON(ClassName)	\
	private:	\
		ClassName() = default;	\
		REMOVE_COPY(ClassName);	\
		REMOVE_MOVE(ClassName);	\
	public:	\
		static ClassName& getInstance()	\
		{	\
			static ClassName instance;	\
			return instance;	\
		}
	

#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif