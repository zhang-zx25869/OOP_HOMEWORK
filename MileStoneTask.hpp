#ifndef MILESTONETASK_HPP
#define MILESTONETASK_HPP

#include "Task.hpp"

class MilestoneTask : public Task
{
    public:
        bool IsBasicTask() const override;

        MilestoneTask(const std::string& Name);
};

#endif