#pragma once
#include <tuple>

namespace keyh
{
    class TaskAction
    {
    public:
        TaskAction() = default;
        template <typename F, typename... Args>
        TaskAction(F&& f, Args&&... args);
        ~TaskAction();

    public:
        TaskAction(const TaskAction&) = delete; 
        TaskAction& operator=(const TaskAction&) = delete;
        TaskAction(TaskAction&& other) noexcept;
        TaskAction& operator=(TaskAction&& other) noexcept;

    private:
        void* _storage = nullptr;
        void (*_invokeFunc)(void*) = nullptr;
        void (*_destructFunc)(void*) = nullptr;

    public:
        void invoke();
        bool isValid() const;
        void reset();

    private:
        template <typename Func, typename... Args>
        class TaskActionStorageImpl
        {
        public:
            TaskActionStorageImpl(Func&& f, Args&&... args);

        private:
            Func _f;
            std::tuple<Args...> _args;

        public:
            static void invoke(void* storage);
            static void destruct(void* storage);
        };
    };
}
#include "TaskAction.hpp"