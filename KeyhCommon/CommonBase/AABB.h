#pragma once
#include "ScalarVectorTypes.h"

namespace keyh
{
	struct AABB
	{
		float3 _min;
		float3 _max;

		AABB() noexcept;
		AABB(const float3& min, const float3& max) noexcept;
	
		bool contains(const float3& point) const noexcept;
		bool contains(const AABB& other) const noexcept;
		bool intersects(const AABB& other) const noexcept;

		AABB& operator+=(const float3& point) noexcept;
		AABB operator+(const float3& point) const noexcept;
	};
}

