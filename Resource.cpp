#include "Resource.hpp"
#include <stdexcept>

Resource::Resource(unsigned int id, const std::string& Name, double unitcost)
{
    if (unitcost < 0)
        throw std::invalid_argument("Resource: unitCost 不能为负数");
    R_ID = id;
    R_Name = Name;
    R_UnitCost = unitcost;
}

unsigned int Resource::GetID() const
{
    return R_ID;
}

void Resource::SetID(unsigned int id)
{
    R_ID = id;
}

const std::string& Resource::GetName() const
{
    return R_Name;
}

double Resource::GetUnitCost() const
{
    return R_UnitCost;
}

void Resource::SetName(const std::string& name)
{
    R_Name = name;
}

void Resource::SetUnitCost(double unitcost)
{
    if (unitcost < 0)
        throw std::invalid_argument("Resource::SetUnitCost: unitCost 不能为负数");
    R_UnitCost = unitcost;
}