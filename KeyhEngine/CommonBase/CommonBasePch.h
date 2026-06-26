#pragma once
#include "MacroCommon.h"

#pragma region Memory
#include <malloc.h>
#include <cstring>
#include <stdio.h>
#pragma endregion

#pragma region Atmoic
#include <atomic>
#pragma endregion

#if defined(KEYH_PLATFORM_WINDOWS)

#pragma region Windows
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#pragma endregion

#elif defined(KEYH_PLATFORM_POSIX)

#pragma region POSIX
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#pragma endregion

#endif