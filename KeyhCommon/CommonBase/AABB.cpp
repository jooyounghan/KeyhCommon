#include "CommonBasePch.h"
#include "AABB.h"

namespace keyh
{
	AABB::AABB() noexcept
		: _min(float3::Max)
		, _max(float3::Min)
	{}

	AABB::AABB(const float3 & min, const float3 & max) noexcept
		: _min(min)
		, _max(max)
	{}

	bool AABB::contains(const float3 & point) const noexcept
	{
		return 
			point._x >= _min._x && point._y >= _min._y && point._z >= _min._z &&
			point._x <= _max._x && point._y <= _max._y && point._z <= _max._z;
	}
	
	bool AABB::contains(const AABB& other) const noexcept
	{
		return
			_min._x <= other._min._x && _min._y <= other._min._y && _min._z <= other._min._z &&
			_max._x >= other._max._x && _max._y >= other._max._y && _max._z >= other._max._z;
	}

	bool AABB::intersects(const AABB& other) const noexcept
	{
		return
			_min._x <= other._max._x && _max._x >= other._min._x &&
			_min._y <= other._max._y && _max._y >= other._min._y &&
			_min._z <= other._max._z && _max._z >= other._min._z;
	}

	AABB& AABB::operator+=(const float3& point) noexcept
	{
		_min._x = MathUtil::min(_min._x, point._x);
		_min._y = MathUtil::min(_min._y, point._y);
		_min._z = MathUtil::min(_min._z, point._z);
		_max._x = MathUtil::max(_max._x, point._x);
		_max._y = MathUtil::max(_max._y, point._y);
		_max._z = MathUtil::max(_max._z, point._z);
		return *this;
	}

	AABB& AABB::operator+=(const AABB& other) noexcept
	{
		_min._x = MathUtil::min(_min._x, other._min._x);
		_min._y = MathUtil::min(_min._y, other._min._y);
		_min._z = MathUtil::min(_min._z, other._min._z);
		_max._x = MathUtil::max(_max._x, other._max._x);
		_max._y = MathUtil::max(_max._y, other._max._y);
		_max._z = MathUtil::max(_max._z, other._max._z);
		return *this;
	}

	AABB AABB::operator+(const float3& point) const noexcept
	{
		AABB result = *this;
		result += point;
		return result;
	}
	AABB AABB::operator+(const AABB& other) const noexcept
	{
		AABB result = *this;
		result += other;
		return result;
	}
}
