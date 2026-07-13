#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>

class Resource
{
    private:
        unsigned int R_ID;
        std::string R_Name;
        double R_UnitCost;

    public:
        Resource(unsigned int id, const std::string& Name, double unitcost);
        ~Resource() = default;

        unsigned int GetID() const;
        void SetID(unsigned int id);

        const std::string& GetName() const;
        double GetUnitCost() const;

        void SetName(const std::string& name);
        void SetUnitCost(double unitcost);

};

#endif