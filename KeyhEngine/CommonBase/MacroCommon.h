#pragma once

#if defined(KEYH_DEBUG) || defined(KEYH_DEV)
#define KEYH_NATVIS
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(_XBOX)
#define KEYH_PLATFORM_WINDOWS
#endif

#if defined(__linux__) || defined(__APPLE__) || defined(__ORBIS__) || defined(__PROSPERO__)
#define KEYH_PLATFORM_POSIX
#endif

#if defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64)
#define KEYH_PLATFORM_X86
#endif

#if defined(_MSC_VER)
#define KEYH_DEBUG_BREAK() __debugbreak()
#else
#define KEYH_DEBUG_BREAK() __builtin_trap()
#endif

#define KEYH_NOOP() ((void)0)