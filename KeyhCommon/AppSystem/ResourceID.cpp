#include "AppSystemPch.h"
#include "ResourceID.h"

namespace keyh
{
    template<typename Tag>
    ResourceID<Tag>::ResourceID(size_t value)
        : _value(value) {}

}