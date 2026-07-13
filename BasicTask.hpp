#ifndef BASICTASK_HPP
#define BASICTASK_HPP

#include "Task.hpp"

class BasicTask : public Task
{
    public:
        bool IsBasicTask() const override;

        BasicTask(unsigned int id, const std::string& Name, unsigned int duration);
};

#endif