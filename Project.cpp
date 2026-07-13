#include "Project.hpp"
#include <stdexcept>
#include <unordered_map>
#include <queue>

// ================================================================
//  构造 / 析构 / 清空
// ================================================================

Project::Project(const std::string& name) : P_Name(name)
{
}

Project::~Project()
{
    Clear();
}

void Project::Clear()
{
    for (unsigned int i = 0; i < P_Tasks.size(); i++)
    {
        delete P_Tasks[i];
    }
        
    for (unsigned int i = 0; i < P_Resources.size(); i++)
    {
        delete P_Resources[i];
    }
        
    for (unsigned int i = 0; i < P_Dependencies.size(); i++)
    {
        delete P_Dependencies[i];
    }
        
    for (unsigned int i = 0; i < P_Assignments.size(); i++)
    {
        delete P_Assignments[i];
    }

    P_Tasks.clear();
    P_Resources.clear();
    P_Dependencies.clear();
    P_Assignments.clear();

    P_TaskIDMap.clear();
    P_ResourceIDMap.clear();

}

// ================================================================
//  项目名称
// ================================================================

const std::string& Project::GetName() const
{
    return P_Name;
}

void Project::SetName(const std::string& name)
{
    P_Name = name;
}

// ================================================================
//  Task 管理
// ================================================================

void Project::AddTask(const std::string& name, unsigned int duration)
{
    unsigned int id = GenerateTaskID();
    AddTask(id, name, duration);
}

void Project::AddTask(unsigned int id, const std::string& name, unsigned int duration)
{
    for (unsigned int i = 0; i < P_Tasks.size(); ++i)
    {
        if (P_Tasks[i]->GetName() == name)
            throw std::invalid_argument("AddTask: 该任务已存在");
    }

    if (P_TaskIDMap.find(id) != P_TaskIDMap.end())
        throw std::invalid_argument("AddTask: ID " + std::to_string(id) + " 已被占用");

    Task* newTask = nullptr;
    if (duration > 0)
        newTask = new BasicTask(id, name, duration);
    else
        newTask = new MilestoneTask(id, name);

    P_Tasks.push_back(newTask);
    P_TaskIDMap[id] = newTask;
}

void Project::RemoveTask(unsigned int index)
{
    if (index >= P_Tasks.size())
        throw std::out_of_range("RemoveTask: 索引 " + std::to_string(index) + " 越界");

    Task* task = P_Tasks[index];

    for (unsigned int i = 0; i < P_Dependencies.size();)
    {
        if (P_Dependencies[i]->GetPredecessor() == task ||
            P_Dependencies[i]->GetSuccessor() == task)
        {
            delete P_Dependencies[i];
            P_Dependencies.erase(P_Dependencies.begin() + i);
        }
        else
        {
            i++;
        }
    }

    for (unsigned int i = 0; i < P_Assignments.size();)
    {
        if (P_Assignments[i]->GetTask() == task)
        {
            delete P_Assignments[i];
            P_Assignments.erase(P_Assignments.begin() + i);
        }
        else
        {
            i++;
        }
    }

    P_TaskIDMap.erase(task->GetID());

    delete task;

    P_Tasks.erase(P_Tasks.begin() + index);
}


Task* Project::GetTask(unsigned int index) const
{
    if (index >= P_Tasks.size())
        throw std::out_of_range("GetTask: 索引 " + std::to_string(index) + " 越界");
    return P_Tasks[index];
}

Task* Project::GetTaskByID(unsigned int id) const
{
    auto it = P_TaskIDMap.find(id);
    if (it == P_TaskIDMap.end())
        throw std::out_of_range("GetTaskByID: ID " + std::to_string(id) + " 不存在");
    return it->second;
}

unsigned int Project::GetTaskCount() const
{
    return static_cast<unsigned int>(P_Tasks.size());
}

std::vector<Task*> Project::GetPredecessors(Task* task) const
{
    std::vector<Task*> result;
    for (unsigned int i = 0; i < P_Dependencies.size(); i++)
    {
        if (P_Dependencies[i]->GetSuccessor() == task)
        {
            result.push_back(P_Dependencies[i]->GetPredecessor());
        }
    }
    return result;
}

std::vector<Task*> Project::GetSuccessors(Task* task) const
{
    std::vector<Task*> result;
    for (unsigned int i = 0; i < P_Dependencies.size(); i++)
    {
        if (P_Dependencies[i]->GetPredecessor() == task)
        {
            result.push_back(P_Dependencies[i]->GetSuccessor());
        }
    }
    return result;
}

int Project::FindTaskIndex(Task* task) const
{
    for (unsigned int i = 0; i < P_Tasks.size(); i++)
    {
        if (P_Tasks[i] == task)
        {
            return static_cast<int>(i);
        }
    }
    throw std::runtime_error("FindTaskIndex: 未找到该 Task");
}

unsigned int Project::GenerateTaskID() const
{
    unsigned int maxID = 0;
    for (unsigned int i = 0; i < P_Tasks.size(); ++i)
    {
        if (P_Tasks[i]->GetID() > maxID)
            maxID = P_Tasks[i]->GetID();
    }
    return maxID + 1;
}

void Project::ModifyTask(unsigned int index, const std::string& name, unsigned int duration)
{
    if (index >= P_Tasks.size())
        throw std::out_of_range("ModifyTask: 索引 " + std::to_string(index) + " 越界");

    for (unsigned int i = 0; i < P_Tasks.size(); ++i)
    {
        if (P_Tasks[i]->GetName() == name && i != index)
            throw std::invalid_argument("ModifyTask: 该任务已存在");
    }

    Task* oldTask = P_Tasks[index];

    if ((oldTask->IsBasicTask() && duration > 0)
        || (!oldTask->IsBasicTask() && duration == 0))
    {
        oldTask->SetName(name);
        oldTask->SetDuration(duration);
        return;
    }

    Task* newTask = nullptr;

    if (duration > 0)
    {
        newTask = new BasicTask(oldTask->GetID(), name, duration);
    }
    else
    {
        newTask = new MilestoneTask(oldTask->GetID(), name);
    }

    for (unsigned int i = 0; i < P_Dependencies.size(); i++)
    {
        if (P_Dependencies[i]->GetPredecessor() == oldTask)
            P_Dependencies[i]->SetPredecessor(newTask);

        if (P_Dependencies[i]->GetSuccessor() == oldTask)
            P_Dependencies[i]->SetSuccessor(newTask);
    }

    for (unsigned int i = 0; i < P_Assignments.size(); i++)
    {
        if (P_Assignments[i]->GetTask() == oldTask)
        {
            P_Assignments[i]->SetTask(newTask);
        }
    }

    P_Tasks[index] = newTask;

    delete oldTask;

}

// ================================================================
//  Resource 管理
// ================================================================

void Project::AddResource(const std::string& name, double unitCost)
{
    unsigned int id = GenerateResourceID();
    AddResource(id, name, unitCost);
}

void Project::AddResource(unsigned int id, const std::string& name, double unitCost)
{
    for (unsigned int i = 0; i < P_Resources.size(); ++i)
    {
        if (P_Resources[i]->GetName() == name)
            throw std::invalid_argument("AddResource: 该资源已存在");
    }

    if (P_ResourceIDMap.find(id) != P_ResourceIDMap.end())
        throw std::invalid_argument("AddResource: ID " + std::to_string(id) + " 已被占用");

    Resource* newRes = new Resource(id, name, unitCost);
    P_Resources.push_back(newRes);
    P_ResourceIDMap[id] = newRes;
}

void Project::RemoveResource(unsigned int index)
{
    if (index >= P_Resources.size())
        throw std::out_of_range("RemoveResource: 索引 " + std::to_string(index) + " 越界");

    Resource* oldRes = P_Resources[index];

    // 删除与该资源相关的所有分配
    for (unsigned int i = 0; i < P_Assignments.size(); )
    {
        if (P_Assignments[i]->GetResource() == oldRes)
        {
            delete P_Assignments[i];
            P_Assignments.erase(P_Assignments.begin() + i);
        }
        else
        {
            ++i;
        }
    }

    P_ResourceIDMap.erase(oldRes->GetID());

    delete oldRes;

    P_Resources.erase(P_Resources.begin() + index);
}

Resource* Project::GetResource(unsigned int index) const
{
    if (index >= P_Resources.size())
        throw std::out_of_range("GetResource: 索引 " + std::to_string(index) + " 越界");
    return P_Resources[index];
}

Resource* Project::GetResourceByID(unsigned int id) const
{
    auto it = P_ResourceIDMap.find(id);
    if (it == P_ResourceIDMap.end())
        throw std::out_of_range("GetResourceByID: ID " + std::to_string(id) + " 不存在");
    return it->second;
}

unsigned int Project::GetResourceCount() const
{
    return static_cast<unsigned int>(P_Resources.size());
}

int Project::FindResourceIndex(Resource* resource) const
{
    for (unsigned int i = 0; i < P_Resources.size(); ++i)
    {
        if (P_Resources[i] == resource)
            return static_cast<int>(i);
    }
    throw std::runtime_error("FindResourceIndex: 未找到该 Resource");
}

unsigned int Project::GenerateResourceID() const
{
    unsigned int maxID = 0;
    for (unsigned int i = 0; i < P_Resources.size(); ++i)
    {
        if (P_Resources[i]->GetID() > maxID)
            maxID = P_Resources[i]->GetID();
    }
    return maxID + 1;
}

void Project::ModifyResource(unsigned int index, const std::string& name, double unitCost)
{
    if (index >= P_Resources.size())
        throw std::out_of_range("ModifyResource: 索引 " + std::to_string(index) + " 越界");

    for (unsigned int i = 0; i < P_Resources.size(); ++i)
    {
        if (P_Resources[i]->GetName() == name && i != index)
            throw std::invalid_argument("ModifyResource: 该资源名已存在");
    }

    P_Resources[index]->SetName(name);
    P_Resources[index]->SetUnitCost(unitCost);
}

// ================================================================
//  Dependency 管理
// ================================================================

void Project::AddDependency(unsigned int predecessorID, unsigned int successorID, Dependency::Type type, int lag)
{
    if (predecessorID >= P_Tasks.size())
        throw std::out_of_range("AddDependency: predecessorID " + std::to_string(predecessorID) + " 越界");
    if (successorID >= P_Tasks.size())
        throw std::out_of_range("AddDependency: successorID " + std::to_string(successorID) + " 越界");

    // 检查重复
    for (unsigned int i = 0; i < P_Dependencies.size(); ++i)
    {
        if (P_Dependencies[i]->GetPredecessor() == P_Tasks[predecessorID]
            && P_Dependencies[i]->GetSuccessor() == P_Tasks[successorID])
            throw std::invalid_argument("AddDependency: 该依赖关系已存在");
    }

    Dependency* dep = new Dependency(P_Tasks[predecessorID],
                                      P_Tasks[successorID],
                                      type, lag);
    P_Dependencies.push_back(dep);
}

void Project::RemoveDependency(unsigned int index)
{
    if (index >= P_Dependencies.size())
        throw std::out_of_range("RemoveDependency: 索引 " + std::to_string(index) + " 越界");
    delete P_Dependencies[index];
    P_Dependencies.erase(P_Dependencies.begin() + index);
}

Dependency* Project::GetDependency(unsigned int index) const
{
    if (index >= P_Dependencies.size())
        throw std::out_of_range("GetDependency: 索引 " + std::to_string(index) + " 越界");
    return P_Dependencies[index];
}

unsigned int Project::GetDependencyCount() const
{
    return static_cast<unsigned int>(P_Dependencies.size());
}

void Project::ModifyDependency(unsigned int index, Dependency::Type type, int lag)
{
    if (index >= P_Dependencies.size())
        throw std::out_of_range("ModifyDependency: 索引 " + std::to_string(index) + " 越界");

    P_Dependencies[index]->SetType(type);
    P_Dependencies[index]->SetLag(lag);
}

// ================================================================
//  Assignment 管理
// ================================================================

void Project::AddAssignment(unsigned int taskID, unsigned int resourceID, unsigned int quantity)
{
    if (taskID >= P_Tasks.size())
        throw std::out_of_range("AddAssignment: taskID " + std::to_string(taskID) + " 越界");
    if (resourceID >= P_Resources.size())
        throw std::out_of_range("AddAssignment: resourceID " + std::to_string(resourceID) + " 越界");
    if (!P_Tasks[taskID]->IsBasicTask())
        throw std::invalid_argument("MileStoneTask 不可占用资源");

    // 检查重复
    for (unsigned int i = 0; i < P_Assignments.size(); ++i)
    {
        if (P_Assignments[i]->GetTask() == P_Tasks[taskID]
            && P_Assignments[i]->GetResource() == P_Resources[resourceID])
            throw std::invalid_argument("AddAssignment: 该资源分配已存在");
    }

    Assignment* newassignment = new Assignment(P_Tasks[taskID],
                                                P_Resources[resourceID],
                                                quantity);
    P_Assignments.push_back(newassignment);
}

void Project::RemoveAssignment(unsigned int index)
{
    if (index >= P_Assignments.size())
        throw std::out_of_range("RemoveAssignment: 索引 " + std::to_string(index) + " 越界");
    delete P_Assignments[index];
    P_Assignments.erase(P_Assignments.begin() + index);
}

void Project::RemoveAssignment(Task* task, Resource* resource)
{
    for (unsigned int i = 0; i < P_Assignments.size(); ++i)
    {
        if (P_Assignments[i]->GetTask() == task && P_Assignments[i]->GetResource() == resource)
        {
            delete P_Assignments[i];
            P_Assignments.erase(P_Assignments.begin() + i);
            return;
        }
    }
    throw std::runtime_error("RemoveAssignment: 未找到匹配的 (Task, Resource) 分配");
}

Assignment* Project::GetAssignment(unsigned int index) const
{
    if (index >= P_Assignments.size())
        throw std::out_of_range("GetAssignment: 索引 " + std::to_string(index) + " 越界");
    return P_Assignments[index];
}

unsigned int Project::GetAssignmentCount() const
{
    return static_cast<unsigned int>(P_Assignments.size());
}

// ================================================================
//  算法：项目合理性验证
// ================================================================

bool Project::IsValid() const
{
    return CheckDependencyReference()
        && CheckDAG()
        && CheckIsolatedTask();
}

// ----------------------------------------------------------------
//  条件①：依赖图无环（复用 TopologicalSort 的 Kahn 算法）
// ----------------------------------------------------------------
bool Project::CheckDAG() const
{
    int n = static_cast<int>(P_Tasks.size());
    if (n == 0) return true;

    std::vector<Task*> sorted = TopologicalSort();
    return static_cast<int>(sorted.size()) == n;
}

// ----------------------------------------------------------------
//  条件②：无孤立 / 悬挂节点（每个 Task 在一条 源→汇 路径上）
// ----------------------------------------------------------------
bool Project::CheckIsolatedTask() const
{
    int n = static_cast<int>(P_Tasks.size());
    if (n == 0) return true;

    std::unordered_map<Task*, int> idx;
    for (int i = 0; i < n; ++i)
        idx[P_Tasks[i]] = i;

    std::vector<int> inDeg(n, 0), outDeg(n, 0);
    std::vector<std::vector<int>> adj(n), revAdj(n);

    for (const Dependency* d : P_Dependencies)
    {
        int u = idx[d->GetPredecessor()];
        int v = idx[d->GetSuccessor()];
        ++outDeg[u];
        ++inDeg[v];
        adj[u].push_back(v);
        revAdj[v].push_back(u);
    }

    std::vector<int> sources, sinks;
    for (int i = 0; i < n; ++i)
    {
        if (inDeg[i] == 0)  sources.push_back(i);
        if (outDeg[i] == 0) sinks.push_back(i);
    }

    if (sources.empty() || sinks.empty())
        return false;

    // 正向 BFS：从源可达
    std::vector<bool> fromSource(n, false);
    std::queue<int> qSrc;
    for (int s : sources) { fromSource[s] = true; qSrc.push(s); }
    while (!qSrc.empty())
    {
        int u = qSrc.front(); qSrc.pop();
        for (int v : adj[u])
            if (!fromSource[v]) { fromSource[v] = true; qSrc.push(v); }
    }

    // 反向 BFS：能到汇
    std::vector<bool> toSink(n, false);
    std::queue<int> qSnk;
    for (int s : sinks) { toSink[s] = true; qSnk.push(s); }
    while (!qSnk.empty())
    {
        int v = qSnk.front(); qSnk.pop();
        for (int u : revAdj[v])
            if (!toSink[u]) { toSink[u] = true; qSnk.push(u); }
    }

    for (int i = 0; i < n; ++i)
        if (!fromSource[i] || !toSink[i])
            return false;

    return true;
}


// ----------------------------------------------------------------
//  条件③：所有 Dependency 引用的 Task 必须存在于 P_Tasks 中
// ----------------------------------------------------------------
bool Project::CheckDependencyReference() const
{
    for (const Dependency* d : P_Dependencies)
    {
        Task* pred = d->GetPredecessor();
        Task* succ = d->GetSuccessor();
        bool predFound = false, succFound = false;
        for (const Task* t : P_Tasks)
        {
            if (t == pred) predFound = true;
            if (t == succ) succFound = true;
        }
        if (!predFound || !succFound)
            return false;
    }
    return true;
}

// ================================================================
//  算法：拓扑排序（用于 CPM 计算）
// ================================================================

std::vector<Task*> Project::TopologicalSort() const
{
    int n = static_cast<int>(P_Tasks.size());
    std::vector<Task*> order;
    if (n == 0) return order;

    // 建立 Task → 索引 映射
    std::unordered_map<Task*, int> idx;
    for (int i = 0; i < n; ++i)
        idx[P_Tasks[i]] = i;

    // 计算入度
    std::vector<int> inDegree(n, 0);
    for (const Dependency* d : P_Dependencies)
        ++inDegree[idx[d->GetSuccessor()]];

    // 入度为 0 的节点入队
    std::queue<int> q;
    for (int i = 0; i < n; ++i)
        if (inDegree[i] == 0)
            q.push(i);

    // Kahn 算法
    while (!q.empty())
    {
        int u = q.front(); q.pop();
        order.push_back(P_Tasks[u]);

        for (const Dependency* d : P_Dependencies)
        {
            if (d->GetPredecessor() != P_Tasks[u])
                continue;
            int v = idx[d->GetSuccessor()];
            if (--inDegree[v] == 0)
                q.push(v);
        }
    }

    return order;
}

// ================================================================
//  算法：关键路径法（CPM）计算
// ================================================================

std::vector<Task*> Project::CalculateCriticalPath()
{
    int n = static_cast<int>(P_Tasks.size());
    std::vector<Task*> criticalPath;
    if (n == 0) return criticalPath;

    // ---- 前提：项目必须合理 ----
    if (!IsValid())
        throw std::runtime_error("CalculateCriticalPath: 项目不合理，无法进行关键路径计算");

    // ---- Step 1：拓扑排序 ----
    std::vector<Task*> topoOrder = TopologicalSort();

    std::unordered_map<Task*, int> idx;
    for (int i = 0; i < n; ++i)
        idx[topoOrder[i]] = i;

    // ---- Step 2：前向传播，计算 ES / EF ----
    for (int i = 0; i < n; ++i)
    {
        Task* curr = topoOrder[i];

        // ES = max( 所有前驱的 EF )，无前驱则 ES = 0
        int es = 0;
        std::vector<Task*> preds = GetPredecessors(curr);
        for (Task* p : preds)
        {
            int ef = p->GetEF();
            if (ef > es) es = ef;
        }

        curr->SetES(es);
        curr->SetEF(es + static_cast<int>(curr->GetDuration()));
    }

    // 项目总工期 = 所有任务的最大 EF
    int projectDuration = 0;
    for (int i = 0; i < n; ++i)
    {
        int ef = topoOrder[i]->GetEF();
        if (ef > projectDuration) projectDuration = ef;
    }

    // ---- Step 3：反向传播，计算 LF / LS ----
    for (int i = n - 1; i >= 0; --i)
    {
        Task* curr = topoOrder[i];
        std::vector<Task*> succs = GetSuccessors(curr);

        int lf;
        if (succs.empty())
        {
            // 无后继 → LF = 项目总工期
            lf = projectDuration;
        }
        else
        {
            // LF = min( 所有后继的 LS )
            lf = succs[0]->GetLS();
            for (unsigned int j = 1; j < succs.size(); ++j)
            {
                int ls = succs[j]->GetLS();
                if (ls < lf) lf = ls;
            }
        }

        curr->SetLF(lf);
        curr->SetLS(lf - static_cast<int>(curr->GetDuration()));
    }

    // ---- Step 4：提取关键路径（ES == LS 的任务，按拓扑序） ----
    for (int i = 0; i < n; ++i)
    {
        Task* t = topoOrder[i];
        if (t->GetES() == t->GetLS())
            criticalPath.push_back(t);
    }

    return criticalPath;
}