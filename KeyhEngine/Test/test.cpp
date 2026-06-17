#include <cstdio>
#include <cstring>
#include <cwchar>
#include "StaticString.h"

using namespace keyh;

static int g_pass = 0;
static int g_fail = 0;

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

static void printSection(const char* name)
{
    std::printf("\n=== %s ===\n", name);
}

static void printSummary()
{
    std::printf("\n==============================\n");
    std::printf("Results: %d passed, %d failed\n", g_pass, g_fail);
    std::printf("==============================\n");
}

static constexpr const char* kShortText = "hello";
static constexpr const char* kLongText = "this string is longer than the sso buffer";
static constexpr const wchar_t* kShortWideText = L"world";
static constexpr const wchar_t* kLongWideText = L"this wide string is longer than the sso buffer";

static void test_StaticString_create_and_length()
{
    printSection("StaticString - create and length");

    StaticString<char> empty;
    CHECK(empty.empty());
    CHECK(empty.length() == 0);
    CHECK(empty.size() == 0);

    StaticString<char> shortString(kShortText);
    CHECK(!shortString.empty());
    CHECK(shortString.length() == 5);
    CHECK(shortString.size() == 5);
    CHECK(std::strcmp(shortString.c_str(), kShortText) == 0);

    StaticString<char> longString(kLongText);
    CHECK(!longString.empty());
    CHECK(longString.length() == std::strlen(kLongText));
    CHECK(longString.size() == std::strlen(kLongText));
    CHECK(std::strcmp(longString.c_str(), kLongText) == 0);
}

static void test_StaticString_copy()
{
    printSection("StaticString - copy");

    StaticString<char> shortSource(kShortText);
    StaticString<char> shortCopy(shortSource);
    CHECK(shortCopy == shortSource);
    shortCopy[0] = 'H';
    CHECK(shortSource[0] == 'h');
    CHECK(shortCopy[0] == 'H');
    CHECK(shortCopy != shortSource);

    StaticString<char> longSource(kLongText);
    StaticString<char> longCopy("seed");
    longCopy = longSource;
    CHECK(longCopy == longSource);
    longCopy[5] = '_';
    CHECK(longSource[5] != '_');
    CHECK(longCopy[5] == '_');
}

static void test_StaticString_move()
{
    printSection("StaticString - move");

    StaticString<char> shortSource(kShortText);
    StaticString<char> shortMoved(keyh::move(shortSource));
    CHECK(std::strcmp(shortMoved.c_str(), kShortText) == 0);
    CHECK(shortMoved.length() == std::strlen(kShortText));
    CHECK(shortSource.empty());
    CHECK(shortSource.length() == 0);

    StaticString<char> longSource(kLongText);
    StaticString<char> longMoved("temporary");
    longMoved = keyh::move(longSource);
    CHECK(std::strcmp(longMoved.c_str(), kLongText) == 0);
    CHECK(longMoved.length() == std::strlen(kLongText));
    CHECK(longSource.empty());
    CHECK(longSource.length() == 0);
}

static void test_StaticString_clear_and_reuse()
{
    printSection("StaticString - clear and reuse");

    StaticString<char> value(kLongText);
    value.clear();
    CHECK(value.empty());
    CHECK(value.length() == 0);
    CHECK(value.size() == 0);

    value = StaticString<char>("abc");
    CHECK(!value.empty());
    CHECK(value.length() == 3);
    CHECK(std::strcmp(value.c_str(), "abc") == 0);
}

static void test_StaticString_index_access()
{
    printSection("StaticString - index access");

    StaticString<char> shortString(kShortText);
    CHECK(shortString[0] == 'h');
    CHECK(shortString[4] == 'o');
    shortString[1] = 'a';
    CHECK(shortString[1] == 'a');
    CHECK(std::strcmp(shortString.c_str(), "hallo") == 0);

    const StaticString<char> longString(kLongText);
    CHECK(longString[0] == 't');
    CHECK(longString[longString.length() - 1] == 'r');
}

static void test_StaticString_wchar()
{
    printSection("StaticString - wchar_t");

    StaticString<wchar_t> shortString(kShortWideText);
    CHECK(shortString.length() == 5);
    CHECK(std::wcscmp(shortString.c_str(), kShortWideText) == 0);

    StaticString<wchar_t> longString(kLongWideText);
    CHECK(longString.length() == std::wcslen(kLongWideText));
    CHECK(std::wcscmp(longString.c_str(), kLongWideText) == 0);

    StaticString<wchar_t> copied(longString);
    CHECK(copied == longString);
    copied[0] = L'T';
    CHECK(longString[0] == L't');
    CHECK(copied[0] == L'T');
}

int main()
{
    test_StaticString_create_and_length();
    test_StaticString_copy();
    test_StaticString_move();
    test_StaticString_clear_and_reuse();
    test_StaticString_index_access();
    test_StaticString_wchar();

    printSummary();
    return g_fail == 0 ? 0 : 1;
}
