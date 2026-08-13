#include "TestCommon.h"
#include "InlinedVectorTest.h"
#include "InlinedVector.h"

using namespace keyh;

namespace
{
	struct HeapValue
	{
		int* value = nullptr;

		HeapValue()
			: value(new int(0)) {}

		explicit HeapValue(int input)
			: value(new int(input)) {}

		HeapValue(const HeapValue& other)
			: value(new int(*other.value)) {}

		HeapValue(HeapValue&& other) noexcept
			: value(other.value)
		{
			other.value = nullptr;
		}

		~HeapValue()
		{
			delete value;
		}
	};
}

void test_InlinedVector_inline_and_heap_growth()
{
	printSection("InlinedVector - inline and heap growth");

	InlinedVector<int, 2> values;
	CHECK(values.empty());
	CHECK(values.size() == 0);
	CHECK(values.capacity() == 2);

	values.push_back(10);
	values.push_back(20);
	CHECK(values.size() == 2);
	CHECK(values.capacity() == 2);

	values.push_back(30);
	CHECK(values.size() == 3);
	CHECK(values.capacity() >= 3);
	CHECK(values[0] == 10);
	CHECK(values[1] == 20);
	CHECK(values[2] == 30);

	values.resize(5);
	CHECK(values.size() == 5);
	CHECK(values[3] == 0);
	CHECK(values[4] == 0);
}

void test_InlinedVector_copy_and_move()
{
	printSection("InlinedVector - copy and move");

	InlinedVector<HeapValue, 2> source;
	source.emplace_back(11);
	source.emplace_back(22);
	source.emplace_back(33);

	InlinedVector<HeapValue, 2> copied(source);
	CHECK(copied.size() == source.size());
	CHECK(*copied[0].value == 11);
	CHECK(*copied[2].value == 33);

	*copied[0].value = 111;
	CHECK(*source[0].value == 11);
	CHECK(*copied[0].value == 111);

	InlinedVector<HeapValue, 2> moved(keyh::move(source));
	CHECK(moved.size() == 3);
	CHECK(source.empty());
	CHECK(*moved[1].value == 22);
	CHECK(*moved[2].value == 33);
}

void test_InlinedVector_erase_and_shrink_to_fit()
{
	printSection("InlinedVector - erase and shrink to fit");

	InlinedVector<int, 2> values;
	values.push_back(1);
	values.push_back(2);
	values.push_back(3);
	values.push_back(4);

	CHECK(values.capacity() >= 4);

	values.erase(1);
	CHECK(values.size() == 3);
	CHECK(values[0] == 1);
	CHECK(values[1] == 3);
	CHECK(values[2] == 4);

	values.pop_back();
	CHECK(values.size() == 2);
	CHECK(values.capacity() >= 3);

	values.shrinkToFit();
	CHECK(values.capacity() == 2);
	CHECK(values[0] == 1);
	CHECK(values[1] == 3);
}
