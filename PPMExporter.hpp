#ifndef PPMEXPORTER_HPP
#define PPMEXPORTER_HPP

#include "Exporter.hpp"
#include <fstream>
#include <string>

class Project;

class PPMExporter : public Exporter
{
    private:
        void WriteProject(std::ofstream& fout, const Project* project) const;
        void WriteTasks(std::ofstream& fout, const Project* project) const;
        void WriteResources(std::ofstream& fout, const Project* project) const;
        void WriteDependencies(std::ofstream& fout, const Project* project) const;
        void WriteAssignments(std::ofstream& fout, const Project* project) const;

    public:
        PPMExporter() = default;
        ~PPMExporter() override = default;

        void Export(const Project* project, const std::string& filename) const override;
};

#endif
