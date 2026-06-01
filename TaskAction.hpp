#include "TaskAction.h"
namespace keyh
{
    template<typename F, typename ...Args>
    TaskAction::TaskAction(F&& f, Args && ...args)
    {
        using TargetTaskType = TaskActionStorageImpl<F, Args...>;

        _storage = _aligned_malloc(sizeof(TargetTaskType), 16);
        new (_storage) TargetTaskType(std::forward<F>(f), std::forward<Args>(args)...);

        _invokeFunc = &TargetTaskType::invoke;
        _destructFunc = &TargetTaskType::destruct;
    }

    template<typename Func, typename ...Args>
    TaskAction::TaskActionStorageImpl<Func, Args...>::TaskActionStorageImpl(Func&& f, Args && ...args)
        : _f(std::forward<Func>(f))
        , _args(std::forward<Args>(args)...)
    {}

    template<typename Func, typename ...Args>
    void TaskAction::TaskActionStorageImpl<Func, Args...>::invoke(void* storage)
    {
        auto* self = static_cast<TaskActionStorageImpl*>(storage);

        std::apply(self->_f, self->_args);
    }
    
    template<typename Func, typename ...Args>
    void TaskAction::TaskActionStorageImpl<Func, Args...>::destruct(void* storage)
    {
        static_cast<TaskActionStorageImpl*>(storage)->~TaskActionStorageImpl();
    }
}