#ifndef EXPORTER_HPP
#define EXPORTER_HPP

#include <string>

class Project;

class Exporter
{
    public:
        Exporter() = default;
        virtual ~Exporter() = default;

        virtual void Export(const Project* project, const std::string& filename) const = 0;
};

#endif
