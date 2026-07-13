#include "PPMImporter.hpp"
#include "Project.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

// ================================================================
//  Import — 主入口：两遍扫描
// ================================================================

void PPMImporter::Import(const std::string& filename, Project& project) const
{
    std::ifstream fin(filename);
    if (!fin.is_open())
        throw std::runtime_error("PPMImporter::Import: 无法打开文件 " + filename);

    // 读取所有非空、非注释行
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(fin, line))
    {
        // 去掉首尾空白
        size_t start = line.find_first_not_of(" \t\r");
        if (start == std::string::npos) continue;   // 空行
        size_t end = line.find_last_not_of(" \t\r");
        line = line.substr(start, end - start + 1);

        if (line.empty() || line[0] == '#') continue; // 注释行

        lines.push_back(line);
    }
    fin.close();

    if (lines.empty())
        throw std::runtime_error("PPMImporter::Import: 文件内容为空");

    // ---- 第一遍：处理 P / T / M / R（不依赖其他实体） ----
    for (size_t i = 0; i < lines.size(); ++i)
    {
        const std::string& ln = lines[i];
        if (ln.empty()) continue;
        char tag = ln[0];

        switch (tag)
        {
        case 'P':
            ReadProject(project, ln);
            break;
        case 'T':
        case 'M':
            ReadTask(project, ln);
            break;
        case 'R':
            ReadResource(project, ln);
            break;
        case 'D':
        case 'A':
            // 第二遍处理
            break;
        default:
            throw std::runtime_error("PPMImporter::Import: 无法识别的行首标记 '"
                                     + std::string(1, tag) + "'");
        }
    }

    // ---- 第二遍：处理 D / A（依赖 Task/Resource 已存在） ----
    for (size_t i = 0; i < lines.size(); ++i)
    {
        const std::string& ln = lines[i];
        if (ln.empty()) continue;
        char tag = ln[0];

        switch (tag)
        {
        case 'D':
            ReadDependency(project, ln);
            break;
        case 'A':
            ReadAssignment(project, ln);
            break;
        default:
            break;
        }
    }
}

// ================================================================
//  ReadProject — 解析 P 行
//  格式：P ProjectName
// ================================================================

void PPMImporter::ReadProject(Project& project, const std::string& line) const
{
    std::istringstream iss(line);
    char tag;
    std::string name;

    iss >> tag >> name;

    if (iss.fail())
        throw std::runtime_error("PPMImporter::ReadProject: 格式错误");

    project.SetName(name);
}

// ================================================================
//  ReadTask — 解析 T / M 行
//  格式：T ID Name Duration  或  M ID Name
// ================================================================

void PPMImporter::ReadTask(Project& project, const std::string& line) const
{
    std::istringstream iss(line);
    char tag;
    unsigned int id;
    std::string name;
    unsigned int duration = 0;

    iss >> tag >> id >> name >> duration;

    if (iss.fail())
        throw std::runtime_error("PPMImporter::ReadTask: 格式错误");

    project.AddTask(id, name, duration);
}

// ================================================================
//  ReadResource — 解析 R 行
//  格式：R ID Name UnitCost
// ================================================================

void PPMImporter::ReadResource(Project& project, const std::string& line) const
{
    std::istringstream iss(line);
    char tag;
    unsigned int id;
    std::string name;
    double unitCost = 0.0;

    iss >> tag >> id >> name >> unitCost;

    if (iss.fail())
        throw std::runtime_error("PPMImporter::ReadResource: 格式错误");

    project.AddResource(id, name, unitCost);
}

// ================================================================
//  ReadDependency — 解析 D 行
//  格式：D PredecessorID SuccessorID Type Lag
// ================================================================

void PPMImporter::ReadDependency(Project& project, const std::string& line) const
{
    std::istringstream iss(line);
    char tag;
    unsigned int predID = 0, succID = 0;
    std::string typeStr;
    int lag = 0;

    iss >> tag >> predID >> succID >> typeStr >> lag;

    if (iss.fail())
        throw std::runtime_error("PPMImporter::ReadDependency: 格式错误");

    // 解析依赖类型
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
        throw std::runtime_error("PPMImporter::ReadDependency: 无法识别的依赖类型 '" + typeStr + "'");

    // 按 PPM ID 查找 Task 指针，再转为容器索引
    Task* pred = project.GetTaskByID(predID);
    Task* succ = project.GetTaskByID(succID);

    int predIndex = project.FindTaskIndex(pred);
    int succIndex = project.FindTaskIndex(succ);

    project.AddDependency(static_cast<unsigned int>(predIndex),
                           static_cast<unsigned int>(succIndex),
                           type, lag);
}

// ================================================================
//  ReadAssignment — 解析 A 行
//  格式：A TaskID ResourceID Quantity
// ================================================================

void PPMImporter::ReadAssignment(Project& project, const std::string& line) const
{
    std::istringstream iss(line);
    char tag;
    unsigned int taskID = 0;
    unsigned int resourceID = 0; 
    unsigned int quantity = 0;

    iss >> tag >> taskID >> resourceID >> quantity;

    if (iss.fail())
        throw std::runtime_error("PPMImporter::ReadAssignment: 格式错误");

    // 按 PPM ID 查找，转为容器索引
    Task* task = project.GetTaskByID(taskID);
    Resource* res = project.GetResourceByID(resourceID);

    int taskIndex = project.FindTaskIndex(task);
    int resIndex  = project.FindResourceIndex(res);

    project.AddAssignment(static_cast<unsigned int>(taskIndex),
                           static_cast<unsigned int>(resIndex),
                           quantity);
}

