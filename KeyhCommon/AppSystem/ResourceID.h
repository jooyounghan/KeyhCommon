#pragma once

namespace keyh
{
    template<typename Tag>
    struct ResourceID
    {
    public:
		ResourceID(size_t value = kInvalidID)
            : _value(value) {}

    public:
        static constexpr size_t kInvalidID = kInvalidSizeT;

    public:
        inline bool isValid() const { return _value != kInvalidID; }
		inline const Tag& getTagData() const { return _data; }

    public:
        inline explicit operator size_t() const { return _value; }
        inline bool operator==(const ResourceID<Tag>& other) const { return _value == other._value; }

    private:
        size_t  _value = kInvalidID;
        Tag     _data;
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
