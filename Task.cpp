#include "Task.hpp"

Task::Task(unsigned int id, const std::string& Name, unsigned int duration)
{
    T_ID = id;
    T_Name = Name;
    T_Duration = duration;
    T_ES = 0;
    T_EF = 0;
    T_LS = 0;
    T_LF = 0;
}

unsigned int Task::GetID() const
{
    return T_ID;
}

void Task::SetID(unsigned int id)
{
    T_ID = id;
}

const std::string& Task::GetName() const
{
    return T_Name;
}

unsigned int Task::GetDuration() const
{
    return T_Duration;
}

void Task::SetName(const std::string& Name)
{
    T_Name = Name;
}

void Task::SetDuration(unsigned int duration)
{
    T_Duration = duration;
}

int Task::GetES() const
{
    return T_ES;
}

int Task::GetEF() const
{
    return T_EF;
}

int Task::GetLS() const
{
    return T_LS;
}

int Task::GetLF() const
{
    return T_LF;
}

void Task::SetES(int es)
{
    T_ES = es;
}

void Task::SetEF(int ef)
{
    T_EF = ef;
}

void Task::SetLS(int ls)
{
    T_LS = ls;
}

void Task::SetLF(int lf)
{
    T_LF = lf;
}