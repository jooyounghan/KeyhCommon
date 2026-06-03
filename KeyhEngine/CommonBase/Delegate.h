#pragma once
#include "TypeCommon.h"
#include "ClassCommon.h"

namespace keyh
{
    template <typename ReturnType, typename... Args>
    class Delegate;

#define DECLARE_DELEGATE(DelegateName, ReturnType, ...)     \
    using DelegateName = Delegate<ReturnType(__VA_ARGS__)>;

    template <typename ReturnType, typename... Args>
    class Delegate<ReturnType(Args...)>
    {
    public:
        Delegate() = default;
        ~Delegate() = default;

    public:
        REMOVE_COPY(Delegate);

    public:
        Delegate(Delegate&& other) noexcept;
        Delegate& operator=(Delegate&& other) noexcept;

    private:
        using StubFuncType = ReturnType(*)(void*, const uint8*, Args&&...);

        void*               _instancePtr        = nullptr;
        alignas(8) uint8    _methodStorage[16]  = {};
        StubFuncType        _stubFunc           = nullptr;
        void (*_destructFunc)(void*)            = nullptr;

    public:
        template <typename TargetClass>
        Delegate& bind(TargetClass* instance, ReturnType(TargetClass::* method)(Args...));
        template <typename F>
        Delegate& bind(F&& callable);

    public:
        inline ReturnType invoke(Args... args) const { return _stubFunc(_instancePtr, _methodStorage, std::forward<Args>(args)...); }
        inline ReturnType operator()(Args... args) const { return invoke(std::forward<Args>(args)...); }
        inline bool isValid() const { return _stubFunc != nullptr; }

    public:
        void reset();
    };
}
#include "Delegate.hpp"