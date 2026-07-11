#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include "Task.hpp"
#include "Resource.hpp"

class Assignment
{
    private:
        Task* A_Task;           // 关联的任务
        Resource* A_Resource;   // 关联的资源
        unsigned int A_Quantity; // 资源数量

    public:
        Assignment(Task* task, Resource* resource, unsigned int quantity);
        ~Assignment() = default;

        Task* GetTask() const;
        Resource* GetResource() const;
        unsigned int GetQuantity() const;

        void SetTask(Task* task);
        void SetQuantity(unsigned int quantity);
        void SetResource(Resource* resource);

};

#endif
