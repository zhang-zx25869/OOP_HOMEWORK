#include "BasicTask.hpp"

BasicTask::BasicTask(const std::string& Name, unsigned int duration)
    : Task(Name, duration)
{
}

bool BasicTask::IsBasicTask() const 
{
    return true;
}