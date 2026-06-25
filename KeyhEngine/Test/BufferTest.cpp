#include "TestCommon.h"
#include "BufferTest.h"
#include "StaticBuffer.h"
#include "DynamicBuffer.h"

using namespace keyh;

namespace
{
    template<typename T>
    class DynamicBufferTestProxy : public DynamicBuffer<T>
    {
    public:
        ~DynamicBufferTestProxy() override
        {
            this->resetImpl();
        }
    };
}

void test_StaticBuffer_basic()
{
    printSection("StaticBuffer - basic");

    StaticBuffer<char, kBuffer256Bytes> buffer;

    CHECK(sizeof(buffer._buffer) == kBuffer256Bytes);
    CHECK(buffer._buffer[0] == 0);
}

void test_DynamicBuffer_allocate_and_grow()
{
    printSection("DynamicBuffer - allocate and grow");

    DynamicBufferTestProxy<char> buffer;

    CHECK(buffer._buffer == nullptr);
    CHECK(buffer._capacity == 0);

    buffer.allocate(8);
    CHECK(buffer._buffer != nullptr);
    CHECK(buffer._capacity == 8);
    CHECK(buffer._buffer[0] == 0);

    buffer._buffer[0] = 'A';
    char* prevBuffer = buffer._buffer;

    buffer.allocate(4);
    CHECK(buffer._buffer == prevBuffer);
    CHECK(buffer._capacity == 8);
    CHECK(buffer._buffer[0] == 'A');

    buffer.allocate(16);
    CHECK(buffer._buffer != prevBuffer);
    CHECK(buffer._capacity == 16);
    CHECK(buffer._buffer[0] == 'A');
}

