#include "Assignment.hpp"
#include <stdexcept>

Assignment::Assignment(Task* task, Resource* resource, unsigned int quantity)
{
    if (task == nullptr)
        throw std::invalid_argument("Assignment: task 不能为 nullptr");
    if (resource == nullptr)
        throw std::invalid_argument("Assignment: resource 不能为 nullptr");

    A_Task = task;
    A_Resource = resource;
    A_Quantity = quantity;
}

Task* Assignment::GetTask() const
{
    return A_Task;
}

Resource* Assignment::GetResource() const
{
    return A_Resource;
}

unsigned int Assignment::GetQuantity() const
{
    return A_Quantity;
}

void Assignment::SetTask(Task* task)
{
    if (task == nullptr)
        throw std::invalid_argument("Assignment::SetTask: task 不能为 nullptr");
    A_Task = task;
}

void Assignment::SetQuantity(unsigned int quantity)
{
    A_Quantity = quantity;
}

void Assignment::SetResource(Resource* resource)
{
    if (resource == nullptr)
        throw std::invalid_argument("Assignment::SetResource: resource 不能为 nullptr");
    A_Resource = resource;
}