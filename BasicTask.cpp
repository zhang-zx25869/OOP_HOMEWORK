#include "BasicTask.hpp"

BasicTask::BasicTask(unsigned int id, const std::string& Name, unsigned int duration)
    : Task(id, Name, duration)
{
}

bool BasicTask::IsBasicTask() const
{
    return true;
}