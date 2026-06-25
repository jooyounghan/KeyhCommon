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

    CHECK(buffer.capacity() == kBuffer256Bytes);
    CHECK(buffer.getBuffer()[0] == 0);
}

void test_DynamicBuffer_allocate_and_grow()
{
    printSection("DynamicBuffer - allocate and grow");

    DynamicBufferTestProxy<char> buffer;

    CHECK(buffer.getBuffer() == nullptr);
    CHECK(buffer.capacity() == 0);

    buffer.allocate(8);
    CHECK(buffer.getBuffer() != nullptr);
    CHECK(buffer.capacity() == 8);
    CHECK(buffer.getBuffer()[0] == 0);

    buffer.writeOne('A');
    char* prevBuffer = buffer.getBuffer();

    buffer.allocate(4);
    CHECK(buffer.getBuffer() == prevBuffer);
    CHECK(buffer.capacity() == 8);
    CHECK(buffer.getBuffer()[0] == 'A');

    buffer.allocate(16);
    CHECK(buffer.getBuffer() != prevBuffer);
    CHECK(buffer.capacity() == 16);
    CHECK(buffer.getBuffer()[0] == 'A');
}

