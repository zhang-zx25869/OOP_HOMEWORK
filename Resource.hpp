#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>

class Resource
{
    private:
        std::string R_Name;
        double R_UnitCost;

    public:
        Resource(const std::string& Name, double unitcost);
        ~Resource() = default;

        const std::string& GetName() const;
        double GetUnitCost() const;

        void SetName(const std::string& name);
        void SetUnitCost(double unitcost);

};

#endif