#include "MileStoneTask.hpp"

MilestoneTask::MilestoneTask(const std::string& Name)
    : Task(Name, 0)
{
}

bool MilestoneTask::IsBasicTask() const
{
    return false;
}