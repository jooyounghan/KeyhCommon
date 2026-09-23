#define DELEGATE_TEMPLATE_TYPE template<typename ReturnType, typename ...Args>
#define DELEGATE_CLASS Delegate<ReturnType(Args...)>

namespace keyh
{
    DELEGATE_TEMPLATE_TYPE
    DELEGATE_CLASS::Delegate(DELEGATE_CLASS&& other) noexcept
        : _instancePtr(other._instancePtr)
        , _stubFunc(other._stubFunc)
    {
        memcpy(_methodStorage, other._methodStorage, sizeof(_methodStorage));
        other.reset();
    }

    DELEGATE_TEMPLATE_TYPE
    DELEGATE_CLASS& DELEGATE_CLASS::operator=(DELEGATE_CLASS&& other) noexcept
    {
        if (this != &other)
        {
            _instancePtr = other._instancePtr;
            memcpy(_methodStorage, other._methodStorage, sizeof(_methodStorage));
            _stubFunc = other._stubFunc;

            other.reset();
        }
        return *this;
    }

    DELEGATE_TEMPLATE_TYPE
    void DELEGATE_CLASS::reset()
    {
        if (_destructFunc != nullptr)
        {
            _destructFunc(_instancePtr);
            _destructFunc = nullptr;
        }

        _instancePtr = nullptr;
        memset(_methodStorage, 0, sizeof(_methodStorage));
        _stubFunc = nullptr;
    }

    DELEGATE_TEMPLATE_TYPE
    template<typename TargetClass>
    DELEGATE_CLASS& DELEGATE_CLASS::bind(TargetClass* instance, ReturnType(TargetClass::* method)(Args...))
    {
        using MemberPtrType = ReturnType(TargetClass::*)(Args...);

        _instancePtr = instance;
        memcpy(_methodStorage, &method, sizeof(MemberPtrType));

        _stubFunc = [](void* instance, const uint8* methodStorage, Args&&... args) -> ReturnType {
            const MemberPtrType& restoredMethod = *reinterpret_cast<const MemberPtrType*>(methodStorage);

            TargetClass* typedInstance = static_cast<TargetClass*>(instance);
            return (typedInstance->*restoredMethod)(keyh::forward<Args>(args)...);
        };
        return *this;
    }

    DELEGATE_TEMPLATE_TYPE
    template<typename F>
    DELEGATE_CLASS& DELEGATE_CLASS::bind(F&& callable)
    {
        using RawF = Decay_t<F>;
        using FuncPtrType = ReturnType(*)(Args...);

        if constexpr (IsConvertible_v<RawF, FuncPtrType>)
        {
            FuncPtrType funcPtr = static_cast<FuncPtrType>(callable);
            memcpy(_methodStorage, &funcPtr, sizeof(FuncPtrType));

            _stubFunc = [](void*, const uint8* methodStorage, Args&&... args) -> ReturnType {
                const FuncPtrType& restoredFunc = *reinterpret_cast<const FuncPtrType*>(methodStorage);
                return restoredFunc(keyh::forward<Args>(args)...);
            };
        }
        else
        {
            _instancePtr = MemoryUtil::alignedAlloc(sizeof(RawF), 16);
            new (_instancePtr) RawF(keyh::forward<F>(callable));

            _stubFunc = [](void* inst, const uint8*, Args&&... args) -> ReturnType {
                return (*reinterpret_cast<RawF*>(inst))(keyh::forward<Args>(args)...);
            };

            _destructFunc = [](void* instance) {
                static_cast<RawF*>(instance)->~RawF();
                MemoryUtil::alignedFree(instance);
            };
        }
        return *this;
    }
}

#undef DELEGATE_TEMPLATE_TYPE
#undef DELEGATE_CLASS