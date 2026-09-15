#include "TestCommon.h"
#include "StaticStringTest.h"
#include "StaticString.h"
#include "StrUtil.h"

using namespace keyh;

static constexpr char kShortText[] = "hello";
static constexpr char kLongText[] = "this string is longer than the sso buffer";
static constexpr wchar_t kShortWideText[] = L"world";
static constexpr wchar_t kLongWideText[] = L"this wide string is longer than the sso buffer";

static_assert((sizeof(kShortText) - 1) < StrUtil::kSsoCapacity);
static_assert((sizeof(kLongText) - 1) >= StrUtil::kSsoCapacity);
static_assert((sizeof(kShortWideText) / sizeof(wchar_t) - 1) < StrUtil::kSsoCapacity);
static_assert((sizeof(kLongWideText) / sizeof(wchar_t) - 1) >= StrUtil::kSsoCapacity);

void test_StaticString_create_and_length()
{
    printSection("StaticString - create and length");

    StaticString<char> empty;
    CHECK(empty.empty());
    CHECK(empty.length() == 0);
    CHECK(empty.size() == 0);

    StaticString<char> shortString(kShortText);
    CHECK(!shortString.empty());
    CHECK(shortString.length() == 5);
    CHECK(shortString.size() == 5 * sizeof(char));
    CHECK(std::strcmp(shortString.c_str(), kShortText) == 0);

    StaticString<char> longString(kLongText);
    CHECK(!longString.empty());
    CHECK(longString.length() == std::strlen(kLongText));
    CHECK(longString.size() == std::strlen(kLongText) * sizeof(char));
    CHECK(std::strcmp(longString.c_str(), kLongText) == 0);
}

void test_StaticString_copy()
{
    printSection("StaticString - copy");

    StaticString<char> shortSource(kShortText);
    StaticString<char> shortCopy(shortSource);
    CHECK(shortCopy == shortSource);
    shortCopy[0] = 'H';
    CHECK(shortSource[0] == 'h');
    CHECK(shortCopy[0] == 'H');
    CHECK(std::strcmp(shortCopy.c_str(), "Hello") == 0);
    CHECK(shortCopy != shortSource);

    StaticString<char> longSource(kLongText);
    StaticString<char> longCopy("seed");
    longCopy = longSource;
    CHECK(longCopy == longSource);
    longCopy[5] = '_';
    CHECK(longSource[5] != '_');
    CHECK(longCopy[5] == '_');
}

void test_StaticString_move()
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

void test_StaticString_clear_and_reuse()
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

void test_StaticString_index_access()
{
    printSection("StaticString - index access");

    StaticString<char> shortString(kShortText);
    CHECK(shortString[0] == 'h');
    CHECK(shortString[4] == 'o');
    shortString[1] = 'u';
    CHECK(shortString[1] == 'u');
    CHECK(std::strcmp(shortString.c_str(), "hullo") == 0);

    const StaticString<char> longString(kLongText);
    CHECK(longString[0] == 't');
    CHECK(longString[longString.length() - 1] == 'r');
}

void test_StaticString_wchar()
{
    printSection("StaticString - wchar_t");

    StaticString<wchar_t> shortString(kShortWideText);
    CHECK(shortString.length() == 5);
    CHECK(shortString.size() == 5 * sizeof(wchar_t));
    CHECK(std::wcscmp(shortString.c_str(), kShortWideText) == 0);

    StaticString<wchar_t> longString(kLongWideText);
    CHECK(longString.length() == std::wcslen(kLongWideText));
    CHECK(longString.size() == std::wcslen(kLongWideText) * sizeof(wchar_t));
    CHECK(std::wcscmp(longString.c_str(), kLongWideText) == 0);

    StaticString<wchar_t> copied(longString);
    CHECK(copied == longString);
    copied[0] = L'T';
    CHECK(longString[0] == L't');
    CHECK(copied[0] == L'T');
}
