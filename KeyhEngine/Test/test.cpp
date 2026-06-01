#include <iostream>
#include "TaskAction.h"

using namespace keyh;

#include <iostream>
#include <chrono>
#include <vector>

class IOldTask
{
public:
    virtual ~IOldTask() = default;
    virtual void invoke() = 0;
};

class OldTaskImpl : public IOldTask
{
public:
    OldTaskImpl(int value) : _value(value) {}
    virtual void invoke() override
    {
        volatile int temp = _value + 1;
    }
private:
    int _value;
};

void benchmarkFunction(int value)
{
    volatile int temp = value + 1;
}

int main()
{
    const size_t taskCount = 10000000;
    std::cout << "Preparing benchmark for " << taskCount << " tasks..." << std::endl;

    std::vector<IOldTask*> oldTasks;
    oldTasks.reserve(taskCount);
    for (size_t i = 0; i < taskCount; ++i)
    {
        oldTasks.push_back(new OldTaskImpl(static_cast<int>(i)));
    }

    std::vector<TaskAction> newTasks;
    newTasks.reserve(taskCount);
    for (size_t i = 0; i < taskCount; ++i)
    {
        newTasks.emplace_back(&benchmarkFunction, static_cast<int>(i));
    }

    std::cout << "\n--- Starting Benchmark ---" << std::endl;

    auto startOld = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < taskCount; ++i)
    {
        oldTasks[i]->invoke();
    }
    auto endOld = std::chrono::high_resolution_clock::now();
    auto durationOld = std::chrono::duration_cast<std::chrono::milliseconds>(endOld - startOld).count();
    std::cout << "1. Virtual Function Style (Old): " << durationOld << " ms" << std::endl;

    auto startNew = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < taskCount; ++i)
    {
        newTasks[i].invoke();
    }
    auto endNew = std::chrono::high_resolution_clock::now();
    auto durationNew = std::chrono::duration_cast<std::chrono::milliseconds>(endNew - startNew).count();
    std::cout << "2. Type Erasure Pointer Style (New): " << durationNew << " ms" << std::endl;

    float improvement = static_cast<float>(durationOld) / static_cast<float>(durationNew);
    std::cout << "\nPerformance Factor: " << improvement << "x faster" << std::endl;

    for (auto* task : oldTasks)
    {
        delete task;
    }

    return 0;
}