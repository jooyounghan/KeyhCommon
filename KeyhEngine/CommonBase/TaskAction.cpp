#include "CommonBasePch.h"
#include "TaskAction.h"

namespace keyh
{
    TaskAction::~TaskAction()
    {
        reset();
    }
    
    TaskAction::TaskAction(TaskAction&& other) noexcept
        : _storage(other._storage)
        , _invokeFunc(other._invokeFunc)
        , _destructFunc(other._destructFunc)
    {
        other._storage = nullptr;
        other._invokeFunc = nullptr;
        other._destructFunc = nullptr;
    }

    TaskAction& TaskAction::operator=(TaskAction&& other) noexcept
    {
        if (this != &other)
        {
            reset();
            _storage = other._storage;
            _invokeFunc = other._invokeFunc;
            _destructFunc = other._destructFunc;

            other._storage = nullptr;
            other._invokeFunc = nullptr;
            other._destructFunc = nullptr;
        }
        return *this;
    }
    
    void TaskAction::invoke()
    {
        if (isValid())
        {
            _invokeFunc(_storage);
        }
    }
    
    bool TaskAction::isValid() const
    {
        return _storage != nullptr && _invokeFunc != nullptr;
    }
    
    void TaskAction::reset()
    {
        if (_storage)
        {
            if (_destructFunc)
            {
                _destructFunc(_storage);
            }
            _aligned_free(_storage);
            _storage = nullptr;
        }
        _invokeFunc = nullptr;
        _destructFunc = nullptr;
    }
}
