#ifndef PPMIMPORTER_HPP
#define PPMIMPORTER_HPP

#include "Importer.hpp"
#include <fstream>
#include <string>

class Project;

class PPMImporter : public Importer
{
    private:
        void ReadProject(std::ifstream& fin, Project* project, const std::string& line) const;
        void ReadTask(Project* project, const std::string& line) const;
        void ReadResource(Project* project, const std::string& line) const;
        void ReadDependency(Project* project, const std::string& line) const;
        void ReadAssignment(Project* project, const std::string& line) const;

    public:
        PPMImporter() = default;
        ~PPMImporter() override = default;

        Project* Import(const std::string& filename) const override;
};


#endif