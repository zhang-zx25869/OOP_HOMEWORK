#include "PPMExporter.hpp"
#include "Project.hpp"
#include "Dependency.hpp"
#include <stdexcept>

// ================================================================
//  Export — 主入口
// ================================================================

void PPMExporter::Export(const Project* project, const std::string& filename) const
{
    std::ofstream fout(filename);
    if (!fout.is_open())
        throw std::runtime_error("PPMExporter::Export: 无法创建文件 " + filename);

    WriteProject(fout, project);
    WriteTasks(fout, project);
    WriteResources(fout, project);
    WriteDependencies(fout, project);
    WriteAssignments(fout, project);

    fout.close();
}

// ================================================================
//  WriteProject — 写入 P 行
// ================================================================

void PPMExporter::WriteProject(std::ofstream& fout, const Project* project) const
{
    fout << "P " << project->GetName() << "\n";
}

// ================================================================
//  WriteTasks — 写入 T / M 行
// ================================================================

void PPMExporter::WriteTasks(std::ofstream& fout, const Project* project) const
{
    unsigned int count = project->GetTaskCount();
    for (unsigned int i = 0; i < count; ++i)
    {
        Task* task = project->GetTask(i);
        if (task->GetDuration() > 0)
            fout << "T " << task->GetID() << " "
                 << task->GetName() << " "
                 << task->GetDuration() << "\n";
        else
            fout << "M " << task->GetID() << " "
                 << task->GetName() << " "
                 << task->GetDuration() << "\n";
    }
}

// ================================================================
//  WriteResources — 写入 R 行
// ================================================================

void PPMExporter::WriteResources(std::ofstream& fout, const Project* project) const
{
    unsigned int count = project->GetResourceCount();
    for (unsigned int i = 0; i < count; ++i)
    {
        Resource* res = project->GetResource(i);
        fout << "R " << res->GetID() << " "
             << res->GetName() << " "
             << res->GetUnitCost() << "\n";
    }
}

// ================================================================
//  WriteDependencies — 写入 D 行
// ================================================================

void PPMExporter::WriteDependencies(std::ofstream& fout, const Project* project) const
{
    unsigned int count = project->GetDependencyCount();
    for (unsigned int i = 0; i < count; ++i)
    {
        Dependency* dep = project->GetDependency(i);

        const char* typeStr = "FS";
        switch (dep->GetType())
        {
        case Dependency::Type::FS: typeStr = "FS"; break;
        case Dependency::Type::SS: typeStr = "SS"; break;
        case Dependency::Type::SF: typeStr = "SF"; break;
        case Dependency::Type::FF: typeStr = "FF"; break;
        }

        fout << "D "
             << dep->GetPredecessor()->GetID() << " "
             << dep->GetSuccessor()->GetID() << " "
             << typeStr << " "
             << dep->GetLag() << "\n";
    }
}

// ================================================================
//  WriteAssignments — 写入 A 行
// ================================================================

void PPMExporter::WriteAssignments(std::ofstream& fout, const Project* project) const
{
    unsigned int count = project->GetAssignmentCount();
    for (unsigned int i = 0; i < count; ++i)
    {
        Assignment* assign = project->GetAssignment(i);
        fout << "A "
             << assign->GetTask()->GetID() << " "
             << assign->GetResource()->GetID() << " "
             << assign->GetQuantity() << "\n";
    }
}
