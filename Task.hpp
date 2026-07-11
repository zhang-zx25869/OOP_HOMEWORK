#ifndef TASK_HPP
#define TASK_HPP

#include <string>

class Task
{
    private:
        std::string T_Name;
        unsigned int T_Duration;

        int T_ES;
        int T_EF;
        int T_LS;
        int T_LF;

    public:
        virtual bool IsBasicTask() const = 0;

        Task(const std::string& Name, unsigned int duration);
        virtual ~Task() = default;

        const std::string& GetName() const;
        unsigned int GetDuration() const;

        void SetName(const std::string& Name);
        void SetDuration(unsigned int duration);

        int GetES() const;
        int GetEF() const;
        int GetLS() const;
        int GetLF() const;

        void SetES(int es);
        void SetEF(int ef);
        void SetLS(int ls);
        void SetLF(int lf);

};

#endif