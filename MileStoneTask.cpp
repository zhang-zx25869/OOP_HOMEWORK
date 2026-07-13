#include "MileStoneTask.hpp"

MilestoneTask::MilestoneTask(unsigned int id, const std::string& Name)
    : Task(id, Name, 0)
{
}

bool MilestoneTask::IsBasicTask() const
{
    return false;
}