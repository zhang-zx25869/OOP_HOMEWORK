#ifndef DEPENDENCY_HPP
#define DEPENDENCY_HPP

#include <string>
#include "Task.hpp"

class Dependency
{
    public:
        enum class Type {FS, SS, SF, FF};
        
    private:
        Task* D_Predecessor;
        Task* D_Successor;
        Type D_Type;
        int D_Lag;

    public:
        Dependency(Task* Predecessor, Task* Successor, Type D_Type, int D_Lag);
        ~Dependency() = default;

        Task* GetPredecessor() const;
        Task* GetSuccessor() const;
        Type GetType() const;
        int GetLag() const;

        void SetPredecessor(Task* task);
        void SetSuccessor(Task* task);
        void SetType(Type type);
        void SetLag(int lag);

};

#endif