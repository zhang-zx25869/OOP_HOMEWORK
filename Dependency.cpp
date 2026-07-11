#include "Dependency.hpp"
#include <stdexcept>

Dependency::Dependency(Task* Predecessor, Task* Successor, Type type, int lag)
{
    if (Predecessor == nullptr)
        throw std::invalid_argument("Dependency: Predecessor 不能为 nullptr");
    if (Successor == nullptr)
        throw std::invalid_argument("Dependency: Successor 不能为 nullptr");

    D_Predecessor = Predecessor;
    D_Successor = Successor;
    D_Type = type;
    D_Lag = lag;
}

Task* Dependency::GetPredecessor() const
{
    return D_Predecessor;
}

Task* Dependency::GetSuccessor() const
{
    return D_Successor;
}

Dependency::Type Dependency::GetType() const
{
    return D_Type;
}

int Dependency::GetLag() const
{
    return D_Lag;
}

void Dependency::SetPredecessor(Task* task)
{
    if (task == nullptr)
        throw std::invalid_argument("Dependency::SetPredecessor: task 不能为 nullptr");
    D_Predecessor = task;
}

void Dependency::SetSuccessor(Task* task)
{
    if (task == nullptr)
        throw std::invalid_argument("Dependency::SetSuccessor: task 不能为 nullptr");
    D_Successor = task;
}

void Dependency::SetType(Type type)
{
    D_Type = type;
}

void Dependency::SetLag(int lag)
{
    D_Lag = lag;
}