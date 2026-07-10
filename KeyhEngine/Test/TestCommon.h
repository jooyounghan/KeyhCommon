#pragma once
#include <cstdio>
#include "CommonBasePch.h"
#include "ReflectSystem.h"

extern int g_pass;
extern int g_fail;

#define CHECK(expr) \
    do { \
        if (expr) { \
            ++g_pass; \
            std::printf("  [PASS] %s\n", #expr); \
        } else { \
            ++g_fail; \
            std::printf("  [FAIL] %s  (line %d)\n", #expr, __LINE__); \
        } \
    } while (0)

inline void printSection(const char* name)
{
    std::printf("\n=== %s ===\n", name);
}

inline void printSummary()
{
    std::printf("\n==============================\n");
    std::printf("Results: %d passed, %d failed\n", g_pass, g_fail);
    std::printf("==============================\n");
}
