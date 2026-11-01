#pragma once

namespace keyh
{
    template<typename Tag>
    struct ResourceID
    {
    public:
		ResourceID(size_t value = kInvalidID);

    public:
        static constexpr size_t kInvalidID = kInvalidSizeT;

        inline constexpr bool isValid() const { return _value != kInvalidID; }
        inline constexpr explicit operator size_t() const { return _value; }

    private:
        size_t _value = kInvalidID;
    };
}

#pragma region FNV1aHash Specializations
namespace keyh
{
    template<typename Tag>
    struct FNV1aHash<ResourceID<Tag>>
    {
        size_t operator()(const ResourceID<Tag>& value) const noexcept
        {
			return FNV1aHasher::hash(reinterpret_cast<const char*>(&value), sizeof(ResourceID<Tag>));
        }
    };
}
#pragma endregion
