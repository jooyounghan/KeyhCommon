#include "TestCommon.h"
#include "StringViewTest.h"
#include "StringView.h"
#include "SplitterString.h"
#include "StrUtil.h"

using namespace keyh;

void test_StringView_removePrefix()
{
    printSection("StringView - removePrefix");

    StringViewA sv("hello world");
    sv.removePrefix(6);
    CHECK(sv.length() == 5);
    CHECK(sv == StringViewA("world"));

    sv.removePrefix(0);
    CHECK(sv.length() == 5);
    CHECK(sv == StringViewA("world"));

    sv.removePrefix(5);
    CHECK(sv.empty());
    CHECK(sv.length() == 0);
}

void test_StringView_removeSuffix()
{
    printSection("StringView - removeSuffix");

    StringViewA sv("hello world");
    sv.removeSuffix(6);
    CHECK(sv.length() == 5);
    CHECK(sv == StringViewA("hello"));

    sv.removeSuffix(0);
    CHECK(sv.length() == 5);
    CHECK(sv == StringViewA("hello"));

    sv.removeSuffix(5);
    CHECK(sv.empty());
    CHECK(sv.length() == 0);
}

void test_StringView_trimStart()
{
    printSection("StringView - trimStart");

    StringViewA sv("   hello   ");
    sv.trimStart();
    CHECK(sv == StringViewA("hello   "));
    CHECK(sv.length() == 8);

    StringViewA sv2("hello");
    sv2.trimStart();
    CHECK(sv2 == StringViewA("hello"));
    CHECK(sv2.length() == 5);

    StringViewA sv3("   ");
    sv3.trimStart();
    CHECK(sv3.empty());
    CHECK(sv3.length() == 0);

    StringViewA sv4("");
    sv4.trimStart();
    CHECK(sv4.empty());
    CHECK(sv4.length() == 0);
}

void test_StringView_trimEnd()
{
    printSection("StringView - trimEnd");

    StringViewA sv("   hello   ");
    sv.trimEnd();
    CHECK(sv == StringViewA("   hello"));
    CHECK(sv.length() == 8);

    StringViewA sv2("hello");
    sv2.trimEnd();
    CHECK(sv2 == StringViewA("hello"));
    CHECK(sv2.length() == 5);

    StringViewA sv3("   ");
    sv3.trimEnd();
    CHECK(sv3.empty());
    CHECK(sv3.length() == 0);

    StringViewA sv4("");
    sv4.trimEnd();
    CHECK(sv4.empty());
    CHECK(sv4.length() == 0);
}

void test_StringView_trim()
{
    printSection("StringView - trim");

    StringViewA sv(" \t\r\n hello world \n\t ");
    sv.trim();
    CHECK(sv == StringViewA("hello world"));
    CHECK(sv.length() == 11);

    StringViewA sv2("hello world");
    sv2.trim();
    CHECK(sv2 == StringViewA("hello world"));
    CHECK(sv2.length() == 11);

    StringViewA sv3("   ");
    sv3.trim();
    CHECK(sv3.empty());
    CHECK(sv3.length() == 0);

    StringViewA sv4("");
    sv4.trim();
    CHECK(sv4.empty());
    CHECK(sv4.length() == 0);
}

void test_StringView_wchar()
{
    printSection("StringView - wchar");

    StringViewW sv(L"  \t wide world \t  ");
    sv.trim();
    CHECK(sv == StringViewW(L"wide world"));
    CHECK(sv.length() == 10);

    sv.removePrefix(5);
    CHECK(sv == StringViewW(L"world"));
    CHECK(sv.length() == 5);

    sv.removeSuffix(2);
    CHECK(sv == StringViewW(L"wor"));
    CHECK(sv.length() == 3);
}

void test_SplitterString_with_trim()
{
    printSection("SplitterString - with trim");

    SplitterStringA splitter(StringViewA(" a , b ,  c  "), ",");
    CHECK(splitter.size() == 3);
    CHECK(splitter[0] == StringViewA("a"));
    CHECK(splitter[1] == StringViewA("b"));
    CHECK(splitter[2] == StringViewA("c"));

    SplitterStringW splitterW(StringViewW(L" alpha ;  beta  ; gamma "), ";");
    CHECK(splitterW.size() == 3);
    CHECK(splitterW[0] == StringViewW(L"alpha"));
    CHECK(splitterW[1] == StringViewW(L"beta"));
    CHECK(splitterW[2] == StringViewW(L"gamma"));
}
