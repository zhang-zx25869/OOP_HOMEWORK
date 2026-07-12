#ifndef IMPORTER_HPP
#define IMPORTER_HPP

#include <string>

class Project;

class Importer
{
    public:
        Importer() = default;
        virtual ~Importer() = default;

        virtual Project* Import(const std::string& filename) const = 0;
};

#endif