#ifndef PPMIMPORTER_HPP
#define PPMIMPORTER_HPP

#include "Importer.hpp"
#include <string>

class Project;

class PPMImporter : public Importer
{
    private:
        void ReadProject(Project& project, const std::string& line) const;
        void ReadTask(Project& project, const std::string& line) const;
        void ReadResource(Project& project, const std::string& line) const;
        void ReadDependency(Project& project, const std::string& line) const;
        void ReadAssignment(Project& project, const std::string& line) const;

    public:
        PPMImporter() = default;
        ~PPMImporter() override = default;

        void Import(const std::string& filename, Project& project) const override;
};


#endif