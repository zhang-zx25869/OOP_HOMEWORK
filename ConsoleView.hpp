#ifndef CONSOLEVIEW_HPP
#define CONSOLEVIEW_HPP

#include <string>

// ================================================================
//  ConsoleView — 控制台界面（MVC 中的 V）
// ================================================================
//  职责：
//  - 显示菜单与操作提示
//  - 获取用户输入
//  - 输出操作结果
//  - 不直接访问任何 Model 类，所有操作通过 Controller 中转
// ================================================================

class ConsoleView
{
    public:
        /// 启动主循环，显示菜单并响应用户操作
        void Run();

    private:
        // ================================================================
        //  菜单显示
        // ================================================================

        /// 主菜单
        int  ShowMainMenu() const;

        /// 子菜单：任务管理，返回用户选择
        int  ShowTaskMenu() const;

        /// 子菜单：依赖管理
        int  ShowDependencyMenu() const;

        /// 子菜单：资源管理
        int  ShowResourceMenu() const;

        // ================================================================
        //  输入辅助
        // ================================================================

        /// 读取一个整数，确保在 [min, max] 范围内
        int          ReadChoice(int min, int max) const;

        /// 读取一行非空字符串
        std::string  ReadLine(const std::string& prompt) const;

        /// 读取一个非负整数
        unsigned int ReadUInt(const std::string& prompt) const;

        /// 读取一个非负浮点数
        double       ReadDouble(const std::string& prompt) const;

        /// 读取一个整数（可为负数，如 Lag）
        int          ReadInt(const std::string& prompt) const;

        // ================================================================
        //  项目管理
        // ================================================================

        void DoNewProject();
        void DoImport();
        void DoExport();

        // ================================================================
        //  任务操作（2.3.1 ~ 2.3.5）
        // ================================================================

        void DoListTasks() const;       // 2.3.1
        void DoDeleteTask();            // 2.3.2
        void DoAddTask();               // 2.3.3
        void DoShowRelations() const;   // 2.3.4
        void DoModifyTask();            // 2.3.5

        // ================================================================
        //  依赖操作（2.3.6 ~ 2.3.8）
        // ================================================================

        void DoListDependencies() const;   // 2.3.6
        void DoDeleteDependency();         // 2.3.7
        void DoAddDependency();            // 2.3.8

        // ================================================================
        //  资源操作（2.3.9 ~ 2.3.10 ~ 2.3.11）
        // ================================================================

        void DoListResources() const;   // 2.3.9
        void DoAddResource();           // 2.3.10
        void DoAddAssignment();         // 2.3.11

        // ================================================================
        //  统计信息（2.4）
        // ================================================================

        void DoStatistics() const;
};

#endif // CONSOLEVIEW_HPP
