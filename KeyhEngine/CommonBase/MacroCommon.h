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