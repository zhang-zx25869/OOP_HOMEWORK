#include "Controller.hpp"
#include "Project.hpp"
#include "PPMImporter.hpp"
#include "PPMExporter.hpp"

#include <stdexcept>

// ================================================================
//  单例实现（Meyers Singleton — C++11 线程安全）
// ================================================================

Controller& Controller::GetInstance()
{
    static Controller instance;
    return instance;
}

// ================================================================
//  构造 / 析构
// ================================================================

Controller::Controller() : m_Project(nullptr)
{
}

// ================================================================
//  内部辅助
// ================================================================

void Controller::EnsureProject() const
{
    if (m_Project == nullptr)
        throw std::runtime_error("Controller: 当前没有打开的项目，请先导入或新建项目");
}

// ================================================================
//  项目管理
// ================================================================

void Controller::NewProject(const std::string& name)
{
    delete m_Project;
    m_Project = new Project(name);
}

void Controller::ImportProject(const std::string& filename)
{
    PPMImporter importer;
    Project* newProject = new Project();

    try
    {
        importer.Import(filename, *newProject);
    }
    catch (...)
    {
        delete newProject;
        throw;
    }

    delete m_Project;
    m_Project = newProject;
}

void Controller::ExportProject(const std::string& filename) const
{
    EnsureProject();

    PPMExporter exporter;
    exporter.Export(m_Project, filename);
}

void Controller::CloseProject()
{
    delete m_Project;
    m_Project = nullptr;
}

bool Controller::HasProject() const
{
    return m_Project != nullptr;
}

std::string Controller::GetProjectName() const
{
    EnsureProject();
    return m_Project->GetName();
}

// ================================================================
//  Task 操作
// ================================================================

unsigned int Controller::GetTaskCount() const
{
    EnsureProject();
    return m_Project->GetTaskCount();
}

std::string Controller::GetTaskName(unsigned int index) const
{
    EnsureProject();
    return m_Project->GetTask(index)->GetName();
}

unsigned int Controller::GetTaskDuration(unsigned int index) const
{
    EnsureProject();
    return m_Project->GetTask(index)->GetDuration();
}

unsigned int Controller::GetTaskID(unsigned int index) const
{
    EnsureProject();
    return m_Project->GetTask(index)->GetID();
}

bool Controller::IsBasicTask(unsigned int index) const
{
    EnsureProject();
    return m_Project->GetTask(index)->IsBasicTask();
}

std::vector<unsigned int> Controller::GetTaskPredecessorIndices(unsigned int index) const
{
    EnsureProject();
    Task* task = m_Project->GetTask(index);
    std::vector<Task*> preds = m_Project->GetPredecessors(task);

    std::vector<unsigned int> indices;
    indices.reserve(preds.size());
    for (unsigned int i = 0; i < preds.size(); ++i)
    {
        int idx = m_Project->FindTaskIndex(preds[i]);
        if (idx >= 0)
            indices.push_back(static_cast<unsigned int>(idx));
    }
    return indices;
}

std::vector<unsigned int> Controller::GetTaskSuccessorIndices(unsigned int index) const
{
    EnsureProject();
    Task* task = m_Project->GetTask(index);
    std::vector<Task*> succs = m_Project->GetSuccessors(task);

    std::vector<unsigned int> indices;
    indices.reserve(succs.size());
    for (unsigned int i = 0; i < succs.size(); ++i)
    {
        int idx = m_Project->FindTaskIndex(succs[i]);
        if (idx >= 0)
            indices.push_back(static_cast<unsigned int>(idx));
    }
    return indices;
}

void Controller::AddTask(const std::string& name, unsigned int duration)
{
    EnsureProject();
    m_Project->AddTask(name, duration);
}

void Controller::RemoveTask(unsigned int index)
{
    EnsureProject();
    m_Project->RemoveTask(index);
}

void Controller::ModifyTask(unsigned int index, const std::string& name, unsigned int duration)
{
    EnsureProject();
    m_Project->ModifyTask(index, name, duration);
}

// ================================================================
//  Dependency 操作
// ================================================================

unsigned int Controller::GetDependencyCount() const
{
    EnsureProject();
    return m_Project->GetDependencyCount();
}

unsigned int Controller::GetDependencyPredIndex(unsigned int index) const
{
    EnsureProject();
    Dependency* dep = m_Project->GetDependency(index);
    int idx = m_Project->FindTaskIndex(dep->GetPredecessor());
    return static_cast<unsigned int>(idx);
}

unsigned int Controller::GetDependencySuccIndex(unsigned int index) const
{
    EnsureProject();
    Dependency* dep = m_Project->GetDependency(index);
    int idx = m_Project->FindTaskIndex(dep->GetSuccessor());
    return static_cast<unsigned int>(idx);
}

std::string Controller::GetDependencyTypeStr(unsigned int index) const
{
    EnsureProject();
    Dependency* dep = m_Project->GetDependency(index);

    switch (dep->GetType())
    {
    case Dependency::Type::FS: return "FS";
    case Dependency::Type::SS: return "SS";
    case Dependency::Type::SF: return "SF";
    case Dependency::Type::FF: return "FF";
    default: return "FS";
    }
}

int Controller::GetDependencyLag(unsigned int index) const
{
    EnsureProject();
    return m_Project->GetDependency(index)->GetLag();
}

void Controller::AddDependency(unsigned int predIndex, unsigned int succIndex,
                               const std::string& typeStr, int lag)
{
    EnsureProject();

    // 解析依赖类型字符串
    Dependency::Type type;
    if (typeStr == "FS")
        type = Dependency::Type::FS;
    else if (typeStr == "SS")
        type = Dependency::Type::SS;
    else if (typeStr == "SF")
        type = Dependency::Type::SF;
    else if (typeStr == "FF")
        type = Dependency::Type::FF;
    else
        throw std::invalid_argument("Controller::AddDependency: 无法识别的依赖类型 '" + typeStr + "'");

    m_Project->AddDependency(predIndex, succIndex, type, lag);
}

void Controller::RemoveDependency(unsigned int index)
{
    EnsureProject();
    m_Project->RemoveDependency(index);
}

// ================================================================
//  Resource 操作
// ================================================================

unsigned int Controller::GetResourceCount() const
{
    EnsureProject();
    return m_Project->GetResourceCount();
}

std::string Controller::GetResourceName(unsigned int index) const
{
    EnsureProject();
    return m_Project->GetResource(index)->GetName();
}

double Controller::GetResourceUnitCost(unsigned int index) const
{
    EnsureProject();
    return m_Project->GetResource(index)->GetUnitCost();
}

unsigned int Controller::GetResourceID(unsigned int index) const
{
    EnsureProject();
    return m_Project->GetResource(index)->GetID();
}

void Controller::AddResource(const std::string& name, double unitCost)
{
    EnsureProject();
    m_Project->AddResource(name, unitCost);
}

// ================================================================
//  Assignment 操作
// ================================================================

unsigned int Controller::GetAssignmentCount() const
{
    EnsureProject();
    return m_Project->GetAssignmentCount();
}

unsigned int Controller::GetAssignmentTaskIndex(unsigned int index) const
{
    EnsureProject();
    Assignment* assign = m_Project->GetAssignment(index);
    int idx = m_Project->FindTaskIndex(assign->GetTask());
    return static_cast<unsigned int>(idx);
}

unsigned int Controller::GetAssignmentResourceIndex(unsigned int index) const
{
    EnsureProject();
    Assignment* assign = m_Project->GetAssignment(index);
    int idx = m_Project->FindResourceIndex(assign->GetResource());
    return static_cast<unsigned int>(idx);
}

unsigned int Controller::GetAssignmentQuantity(unsigned int index) const
{
    EnsureProject();
    return m_Project->GetAssignment(index)->GetQuantity();
}

void Controller::AddAssignment(unsigned int taskIndex, unsigned int resourceIndex,
                               unsigned int quantity)
{
    EnsureProject();
    m_Project->AddAssignment(taskIndex, resourceIndex, quantity);
}

unsigned int Controller::GetTotalDuration() const
{
    EnsureProject();

    // 遍历所有任务取最大 EF（CPM 未计算时 EF = 0，返回 0）
    unsigned int count = m_Project->GetTaskCount();
    int maxEF = 0;
    for (unsigned int i = 0; i < count; ++i)
    {
        int ef = m_Project->GetTask(i)->GetEF();
        if (ef > maxEF) maxEF = ef;
    }
    return static_cast<unsigned int>(maxEF);
}

bool Controller::ValidateProject() const
{
    EnsureProject();
    return m_Project->IsValid();
}

std::vector<unsigned int> Controller::CalculateCPM()
{
    EnsureProject();

    std::vector<Task*> criticalTasks = m_Project->CalculateCriticalPath();

    std::vector<unsigned int> ids;
    ids.reserve(criticalTasks.size());
    for (Task* t : criticalTasks)
        ids.push_back(t->GetID());

    return ids;
}
