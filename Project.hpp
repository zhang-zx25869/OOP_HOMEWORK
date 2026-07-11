#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <vector>
#include <string>

#include "Task.hpp"
#include "Resource.hpp"
#include "Dependency.hpp"
#include "Assignment.hpp"

class Project
{
private:
    std::string P_Name;

    std::vector<Task*>       P_Tasks;
    std::vector<Resource*>   P_Resources;
    std::vector<Dependency*> P_Dependencies;
    std::vector<Assignment*> P_Assignments;

    // ---- IsValid 子检测 ----
    bool CheckDependencyReference() const;
    bool CheckDAG() const;
    bool CheckIsolatedTask() const;

public:
    Project(const std::string& name = "");
    ~Project();

    void Clear();

    // ---- 项目名称 ----
    const std::string& GetName() const;
    void SetName(const std::string& name);

    // ================================================================
    //  Task 管理
    // ================================================================

    void AddTask(const std::string& name, unsigned int duration);
    void RemoveTask(unsigned int index);

    Task* GetTask(unsigned int index) const;
    unsigned int GetTaskCount() const;
    std::vector<Task*> GetPredecessors(Task* task) const;
    std::vector<Task*> GetSuccessors(Task* task) const;

    int FindTaskIndex(Task* task) const;

    void ModifyTask(unsigned int index, const std::string& name, unsigned int duration);

    // ================================================================
    //  Resource 管理
    // ================================================================

    void AddResource(const std::string& name, double unitCost);
    void RemoveResource(unsigned int index);

    Resource* GetResource(unsigned int index) const;
    unsigned int GetResourceCount() const;
    
    int FindResourceIndex(Resource* resource) const;

    void ModifyResource(unsigned int index, const std::string& name, double unitCost);

    // ================================================================
    //  Dependency 管理
    // ================================================================
    void AddDependency(unsigned int predecessorID, unsigned int successorID, Dependency::Type type, int lag);
    void RemoveDependency(unsigned int index);

    Dependency* GetDependency(unsigned int index) const;
    unsigned int GetDependencyCount() const;

    void ModifyDependency(unsigned int index, Dependency::Type type, int lag);

    // ================================================================
    //  Assignment 管理
    // ================================================================

    void AddAssignment(unsigned int taskID, unsigned int resourceID, unsigned int quantity);
    void RemoveAssignment(unsigned int index);
    void RemoveAssignment(Task* task, Resource* resource);

    Assignment* GetAssignment(unsigned int index) const;
    unsigned int GetAssignmentCount() const;

    // ================================================================
    //  算法：拓扑排序 & 关键路径
    // ================================================================

    /// 按依赖关系拓扑排序，返回排序后的 Task* 序列
    std::vector<Task*> TopologicalSort() const;

    /// 计算所有任务的 ES/EF/LS/LF，并返回关键路径上的任务
    std::vector<Task*> CalculateCriticalPath();

    /// 验证项目合理性（无环 / 无孤立节点 / Dependency 引用有效）
    bool IsValid() const;

};

#endif
