#pragma once

namespace keyh
{
    template<typename T>
    void ArenaAllocator::invokeDestructor(void* object)
    {
        static_cast<T*>(object)->~T();
    }

    template<typename T, typename ...Args>
    T* ArenaAllocator::createObject(Args && ...args)
    {
        void* memory = allocateMemory(sizeof(T));
        T* object = new (memory) T(keyh::forward<Args>(args)...);

        if constexpr (!std::is_trivially_destructible<T>::value)
        {
            registerDeleter(object, &invokeDestructor<T>);
        }

        return object;
    }
}