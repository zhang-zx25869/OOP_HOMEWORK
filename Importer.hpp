#ifndef IMPORTER_HPP
#define IMPORTER_HPP

#include <string>

class Project;

class Importer
{
    public:
        Importer() = default;
        virtual ~Importer() = default;

        virtual void Import(const std::string& filename, Project& project) const = 0;
};

#endif