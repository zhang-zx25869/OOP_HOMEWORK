#include "ConsoleView.hpp"
#include "Controller.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>

// ================================================================
//  Run — 主循环
// ================================================================

void ConsoleView::Run()
{
    while (true)
    {
        int choice = ShowMainMenu();
        try
        {
            switch (choice)
            {
            case 1:  DoNewProject();       break;
            case 2:  DoImport();           break;
            case 3:  DoExport();           break;
            case 4:
                {
                    bool back = false;
                    Controller& c = Controller::GetInstance();
                    if (!c.HasProject())
                    {
                        std::cout << "请先新建或导入项目。\n";
                        break;
                    }
                    while (!back)
                    {
                        int taskChoice = ShowTaskMenu();
                        try
                        {
                            switch (taskChoice)
                            {
                            case 1: DoListTasks();            break;
                            case 2: DoAddTask();             break;
                            case 3: DoDeleteTask();          break;
                            case 4: DoShowRelations();       break;
                            case 5: DoModifyTask();          break;
                            case 0: back = true;             break;
                            }
                        }
                        catch (const std::exception& e)
                        {
                            std::cout << "错误：" << e.what() << "\n";
                        }
                    }
                }
                break;
            case 5:
                {
                    bool back = false;
                    Controller& c = Controller::GetInstance();
                    if (!c.HasProject())
                    {
                        std::cout << "请先新建或导入项目。\n";
                        break;
                    }
                    while (!back)
                    {
                        int depChoice = ShowDependencyMenu();
                        try
                        {
                            switch (depChoice)
                            {
                            case 1: DoListDependencies();    break;
                            case 2: DoAddDependency();       break;
                            case 3: DoDeleteDependency();    break;
                            case 0: back = true;             break;
                            }
                        }
                        catch (const std::exception& e)
                        {
                            std::cout << "错误：" << e.what() << "\n";
                        }
                    }
                }
                break;
            case 6:
                {
                    bool back = false;
                    Controller& c = Controller::GetInstance();
                    if (!c.HasProject())
                    {
                        std::cout << "请先新建或导入项目。\n";
                        break;
                    }
                    while (!back)
                    {
                        int resChoice = ShowResourceMenu();
                        try
                        {
                            switch (resChoice)
                            {
                            case 1: DoListResources();       break;
                            case 2: DoAddResource();         break;
                            case 3: DoAddAssignment();       break;
                            case 0: back = true;             break;
                            }
                        }
                        catch (const std::exception& e)
                        {
                            std::cout << "错误：" << e.what() << "\n";
                        }
                    }
                }
                break;
            case 7:  DoStatistics();        break;
            case 0:  return;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "错误：" << e.what() << "\n";
        }
    }
}

// ================================================================
//  菜单显示
// ================================================================

int ConsoleView::ShowMainMenu() const
{
    Controller& c = Controller::GetInstance();

    std::cout << "\n";
    std::cout << "=== 项目管理系统 ======================================\n";

    if (c.HasProject())
        std::cout << "当前项目：" << c.GetProjectName() << "\n";
    else
        std::cout << "当前项目：未打开\n";

    std::cout << "------------------------------------------------------\n";
    std::cout << " 1. 新建项目         5. 依赖管理\n";
    std::cout << " 2. 导入 PPM 文件    6. 资源管理\n";
    std::cout << " 3. 导出 PPM 文件    7. 统计信息\n";
    std::cout << " 4. 任务管理         0. 退出\n";
    std::cout << "======================================================\n";

    return ReadChoice(0, 7);
}

int ConsoleView::ShowTaskMenu() const
{
    std::cout << "\n";
    std::cout << "--- 任务管理 ---\n";
    std::cout << " 1. 列出所有任务     4. 查看前驱/后继\n";
    std::cout << " 2. 添加任务         5. 修改任务\n";
    std::cout << " 3. 删除任务         0. 返回\n";

    return ReadChoice(0, 5);
}

int ConsoleView::ShowDependencyMenu() const
{
    std::cout << "\n";
    std::cout << "--- 依赖管理 ---\n";
    std::cout << " 1. 列出所有依赖     3. 删除依赖\n";
    std::cout << " 2. 添加依赖         0. 返回\n";

    return ReadChoice(0, 3);
}

int ConsoleView::ShowResourceMenu() const
{
    std::cout << "\n";
    std::cout << "--- 资源管理 ---\n";
    std::cout << " 1. 列出所有资源     3. 资源分配\n";
    std::cout << " 2. 添加资源         0. 返回\n";

    return ReadChoice(0, 3);
}

// ================================================================
//  输入辅助
// ================================================================

int ConsoleView::ReadChoice(int min, int max) const
{
    int value;
    while (true)
    {
        std::cout << "请输入选项 [" << min << "-" << max << "]：";
        std::cin >> value;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请输入整数。\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (value < min || value > max)
        {
            std::cout << "输入超出范围 [" << min << "-" << max << "]。\n";
            continue;
        }

        return value;
    }
}

std::string ConsoleView::ReadLine(const std::string& prompt) const
{
    std::string line;
    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, line);

        if (!line.empty()) return line;

        std::cout << "输入不能为空，请重新输入。\n";
    }
}

unsigned int ConsoleView::ReadUInt(const std::string& prompt) const
{
    unsigned int value;
    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请输入非负整数。\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

double ConsoleView::ReadDouble(const std::string& prompt) const
{
    double value;
    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请输入数字。\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

int ConsoleView::ReadInt(const std::string& prompt) const
{
    int value;
    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请输入整数。\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

// ================================================================
//  项目管理
// ================================================================

void ConsoleView::DoNewProject()
{
    std::string name = ReadLine("请输入项目名称：");
    Controller::GetInstance().NewProject(name);
    std::cout << "项目 \"" << name << "\" 已创建。\n";
}

void ConsoleView::DoImport()
{
    std::string filename = ReadLine("请输入 PPM 文件路径：");
    Controller::GetInstance().ImportProject(filename);
    std::cout << "项目已导入：" << Controller::GetInstance().GetProjectName() << "\n";
}

void ConsoleView::DoExport()
{
    Controller& c = Controller::GetInstance();
    if (!c.HasProject())
    {
        std::cout << "请先新建或导入项目。\n";
        return;
    }

    std::string filename = ReadLine("请输入导出文件路径：");
    c.ExportProject(filename);
    std::cout << "项目已导出到 " << filename << "\n";
}

// ================================================================
//  任务操作（2.3.1 ~ 2.3.5）
// ================================================================

void ConsoleView::DoListTasks() const
{
    Controller& c = Controller::GetInstance();
    unsigned int count = c.GetTaskCount();

    if (count == 0)
    {
        std::cout << "当前没有任务。\n";
        return;
    }

    std::cout << "\n";
    std::cout << "--- 任务列表（共 " << count << " 个） ---\n";
    std::cout << "索引\tID\t类型\t名称\t工期\n";

    for (unsigned int i = 0; i < count; ++i)
    {
        std::string type = c.IsBasicTask(i) ? "普通任务" : "里程碑";
        std::cout << i << "\t"
                  << c.GetTaskID(i) << "\t"
                  << type << "\t"
                  << c.GetTaskName(i) << "\t"
                  << c.GetTaskDuration(i) << "\n";
    }
}

void ConsoleView::DoAddTask()
{
    std::string name     = ReadLine("请输入任务名称：");
    unsigned int duration = ReadUInt("请输入工期（天）：");

    Controller::GetInstance().AddTask(name, duration);

    if (duration == 0)
        std::cout << "已添加里程碑任务 \"" << name << "\"。\n";
    else
        std::cout << "已添加普通任务 \"" << name << "\"，工期 " << duration << " 天。\n";
}

void ConsoleView::DoDeleteTask()
{
    Controller& c = Controller::GetInstance();
    unsigned int count = c.GetTaskCount();
    if (count == 0)
    {
        std::cout << "当前没有任务可删除。\n";
        return;
    }

    DoListTasks();
    unsigned int index = ReadUInt("请输入要删除的任务索引：");

    std::string name = c.GetTaskName(index);
    c.RemoveTask(index);
    std::cout << "已删除任务 \"" << name << "\"（关联的依赖与分配已自动清理）。\n";
}

void ConsoleView::DoShowRelations() const
{
    Controller& c = Controller::GetInstance();
    unsigned int count = c.GetTaskCount();
    if (count == 0)
    {
        std::cout << "当前没有任务。\n";
        return;
    }

    DoListTasks();
    unsigned int index = ReadUInt("请输入要查看的任务索引：");

    std::vector<unsigned int> preds = c.GetTaskPredecessorIndices(index);
    std::vector<unsigned int> succs = c.GetTaskSuccessorIndices(index);

    std::cout << "\n--- 任务 #" << index << " 的关系 ---\n";
    std::cout << "名称：" << c.GetTaskName(index) << "\n";

    std::cout << "前驱任务（" << preds.size() << " 个）：";
    if (preds.empty())
        std::cout << "无\n";
    else
    {
        std::cout << "\n";
        for (unsigned int i = 0; i < preds.size(); ++i)
            std::cout << "  索引 " << preds[i] << "  " << c.GetTaskName(preds[i]) << "\n";
    }

    std::cout << "后继任务（" << succs.size() << " 个）：";
    if (succs.empty())
        std::cout << "无\n";
    else
    {
        std::cout << "\n";
        for (unsigned int i = 0; i < succs.size(); ++i)
            std::cout << "  索引 " << succs[i] << "  " << c.GetTaskName(succs[i]) << "\n";
    }
}

void ConsoleView::DoModifyTask()
{
    Controller& c = Controller::GetInstance();
    unsigned int count = c.GetTaskCount();
    if (count == 0)
    {
        std::cout << "当前没有任务可修改。\n";
        return;
    }

    DoListTasks();
    unsigned int index    = ReadUInt("请输入要修改的任务索引：");
    std::string  name     = ReadLine("请输入新名称：");
    unsigned int duration = ReadUInt("请输入新工期（天）：");

    c.ModifyTask(index, name, duration);
    std::cout << "任务 #" << index << " 已更新。\n";
}

// ================================================================
//  依赖操作（2.3.6 ~ 2.3.8）
// ================================================================

void ConsoleView::DoListDependencies() const
{
    Controller& c = Controller::GetInstance();
    unsigned int count = c.GetDependencyCount();

    if (count == 0)
    {
        std::cout << "当前没有依赖关系。\n";
        return;
    }

    std::cout << "\n";
    std::cout << "--- 依赖列表（共 " << count << " 个） ---\n";
    std::cout << "依赖序号\t索引\t前置任务\t后置任务\t类型\t时差\n";

    for (unsigned int i = 0; i < count; ++i)
    {
        unsigned int pred = c.GetDependencyPredIndex(i);
        unsigned int succ = c.GetDependencySuccIndex(i);

        std::cout << i << "\t"
                  << pred << "(" << c.GetTaskName(pred) << ")\t"
                  << succ << "(" << c.GetTaskName(succ) << ")\t"
                  << c.GetDependencyTypeStr(i) << "\t"
                  << c.GetDependencyLag(i) << "\n";
    }
}

void ConsoleView::DoAddDependency()
{
    Controller& c = Controller::GetInstance();
    unsigned int taskCount = c.GetTaskCount();
    if (taskCount < 2)
    {
        std::cout << "至少需要 2 个任务才能添加依赖关系。\n";
        return;
    }

    DoListTasks();

    unsigned int pred  = ReadUInt("请输入前置任务索引：");
    unsigned int succ  = ReadUInt("请输入后置任务索引：");

    std::cout << "依赖类型：FS(完成-开始) / SS(开始-开始) / SF(开始-完成) / FF(完成-完成)\n";
    std::string typeStr = ReadLine("请输入依赖类型：");

    int lag = ReadInt("请输入时差 Lag（可为负数表示提前）：");

    c.AddDependency(pred, succ, typeStr, lag);
    std::cout << "已添加依赖：Task#" << pred << " → Task#" << succ
              << "（" << typeStr << "，Lag=" << lag << "）\n";
}

void ConsoleView::DoDeleteDependency()
{
    Controller& c = Controller::GetInstance();
    unsigned int count = c.GetDependencyCount();
    if (count == 0)
    {
        std::cout << "当前没有依赖可删除。\n";
        return;
    }

    DoListDependencies();
    unsigned int index = ReadUInt("请输入要删除的依赖索引：");
    c.RemoveDependency(index);
    std::cout << "依赖 #" << index << " 已删除。\n";
}

// ================================================================
//  资源操作（2.3.9 ~ 2.3.11）
// ================================================================

void ConsoleView::DoListResources() const
{
    Controller& c = Controller::GetInstance();
    unsigned int count = c.GetResourceCount();

    if (count == 0)
    {
        std::cout << "当前没有资源。\n";
        return;
    }

    std::cout << "\n";
    std::cout << "--- 资源列表（共 " << count << " 个） ---\n";
    std::cout << "索引\tID\t名称\t单位成本\n";

    for (unsigned int i = 0; i < count; ++i)
    {
        std::cout << i << "\t"
                  << c.GetResourceID(i) << "\t"
                  << c.GetResourceName(i) << "\t"
                  << c.GetResourceUnitCost(i) << "\n";
    }
}

void ConsoleView::DoAddResource()
{
    std::string name = ReadLine("请输入资源名称：");

    double unitCost;
    while (true)
    {
        unitCost = ReadDouble("请输入单位成本：");
        if (unitCost < 0.0)
        {
            std::cout << "单位成本不能为负数。\n";
            continue;
        }
        break;
    }

    Controller::GetInstance().AddResource(name, unitCost);
    std::cout << "已添加资源 \"" << name << "\"，单位成本 " << unitCost << "。\n";
}

void ConsoleView::DoAddAssignment()
{
    Controller& c = Controller::GetInstance();

    unsigned int taskCount = c.GetTaskCount();
    unsigned int resCount  = c.GetResourceCount();

    bool hasProblem = false;

    if (taskCount == 0)
    {
        std::cout << "当前没有任务。\n";
        hasProblem = true;
    }
    if (resCount == 0)
    {
        std::cout << "当前没有资源。请先添加资源。\n";
        hasProblem = true;
    }
    if (hasProblem)
        return;

    DoListTasks();
    unsigned int taskIndex     = ReadUInt("请输入任务索引：");

    DoListResources();
    unsigned int resourceIndex = ReadUInt("请输入资源索引：");
    unsigned int quantity      = ReadUInt("请输入分配数量：");

    c.AddAssignment(taskIndex, resourceIndex, quantity);
    std::cout << "已分配 Resource#" << resourceIndex
              << " → Task#" << taskIndex
              << "（数量 " << quantity << "）。\n";
}

// ================================================================
//  统计信息（2.4）
// ================================================================

void ConsoleView::DoStatistics() const
{
    Controller& c = Controller::GetInstance();
    if (!c.HasProject())
    {
        std::cout << "请先新建或导入项目。\n";
        return;
    }

    std::cout << "\n";
    std::cout << "=== 项目统计信息 ======================================\n";
    std::cout << "项目名称：" << c.GetProjectName() << "\n";
    std::cout << "Task 总数：" << c.GetTaskCount() << "\n";
    std::cout << "Dependency 总数：" << c.GetDependencyCount() << "\n";
    std::cout << "Resource 总数：" << c.GetResourceCount() << "\n";
    std::cout << "Assignment 总数：" << c.GetAssignmentCount() << "\n";
    std::cout << "项目总工期：" << c.GetTotalDuration() << " 天\n";
    std::cout << "======================================================\n";
}
