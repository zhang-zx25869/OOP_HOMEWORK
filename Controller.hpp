#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>
#include <vector>

class Project;

// ================================================================
//  Controller — 单例控制器（MVC 中的 C）
// ================================================================
//  职责：
//  - 接收来自 View 的用户操作请求
//  - 调用 Model（Project 等可重用类）完成业务逻辑
//  - 调用 Importer/Exporter 完成文件读写
//  - 协调 View 与 Model，是两者之间的唯一桥梁
// ================================================================

class Controller
{
    private:
        // ---- 单例：构造/析构私有 ----
        Controller();
        ~Controller() = default;

        // ---- 内部辅助 ----
        void EnsureProject() const;

        Project* m_Project;

    public:
        // ---- 单例访问 ----
        static Controller& GetInstance();

        // 禁止拷贝与赋值
        Controller(const Controller&) = delete;
        Controller& operator=(const Controller&) = delete;

        // ================================================================
        //  项目管理
        // ================================================================

        void NewProject(const std::string& name);
        void ImportProject(const std::string& filename);
        void ExportProject(const std::string& filename) const;
        void CloseProject();
        bool HasProject() const;
        std::string GetProjectName() const;

        /// 项目总工期（关键路径长度，CPM 未计算时返回 0）
        unsigned int GetTotalDuration() const;

        /// 验证项目合理性（2.5.1）：DAG / 无孤立节点 / 依赖引用有效
        bool ValidateProject() const;

        /// 执行 CPM 关键路径计算，返回关键路径任务 ID 列表（2.5.2）
        std::vector<unsigned int> CalculateCPM();

        // ================================================================
        //  Task 操作（2.3.1 ~ 2.3.5）
        // ================================================================

        unsigned int GetTaskCount() const;
        std::string  GetTaskName(unsigned int index) const;
        unsigned int GetTaskDuration(unsigned int index) const;
        unsigned int GetTaskID(unsigned int index) const;
        bool         IsBasicTask(unsigned int index) const;

        /// 返回指定 Task 所有前驱任务的容器索引（2.3.4）
        std::vector<unsigned int> GetTaskPredecessorIndices(unsigned int index) const;
        /// 返回指定 Task 所有后继任务的容器索引（2.3.4）
        std::vector<unsigned int> GetTaskSuccessorIndices(unsigned int index) const;
        /// 添加 Task（2.3.3）：duration==0 自动创建 MilestoneTask，否则 BasicTask
        void AddTask(const std::string& name, unsigned int duration);
        /// 删除 Task，级联清理关联的 Dependency 与 Assignment（2.3.2）
        void RemoveTask(unsigned int index);
        /// 修改 Task 名称/工期，自动处理 BasicTask ↔ MilestoneTask 类型转换（2.3.5）
        void ModifyTask(unsigned int index, const std::string& name, unsigned int duration);

        // ================================================================
        //  Dependency 操作（2.3.6 ~ 2.3.8）
        // ================================================================

        unsigned int GetDependencyCount() const;
        unsigned int GetDependencyPredIndex(unsigned int index) const;
        unsigned int GetDependencySuccIndex(unsigned int index) const;
        std::string  GetDependencyTypeStr(unsigned int index) const;
        int          GetDependencyLag(unsigned int index) const;

        /// 添加依赖关系（2.3.8）：自动检测重复与 DAG 约束
        void AddDependency(unsigned int predIndex, unsigned int succIndex,
                           const std::string& typeStr, int lag);

        /// 删除依赖关系（2.3.7）
        void RemoveDependency(unsigned int index);

        // ================================================================
        //  Resource 操作（2.3.9 ~ 2.3.10）
        // ================================================================

        unsigned int GetResourceCount() const;
        std::string  GetResourceName(unsigned int index) const;
        double       GetResourceUnitCost(unsigned int index) const;
        unsigned int GetResourceID(unsigned int index) const;

        /// 添加资源（2.3.10）
        void AddResource(const std::string& name, double unitCost);

        // ================================================================
        //  Assignment 操作（2.3.11）
        // ================================================================

        unsigned int GetAssignmentCount() const;
        unsigned int GetAssignmentTaskIndex(unsigned int index) const;
        unsigned int GetAssignmentResourceIndex(unsigned int index) const;
        unsigned int GetAssignmentQuantity(unsigned int index) const;

        /// 为 Task 分配 Resource（2.3.11）
        void AddAssignment(unsigned int taskIndex, unsigned int resourceIndex,
                           unsigned int quantity);

};

#endif 
