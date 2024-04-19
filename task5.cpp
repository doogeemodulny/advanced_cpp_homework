#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime> 


enum LogStatus {LOG_NORMAL, LOG_WARNING, LOG_ERROR};
//c C++20 можно, использовав enum class, 
// подключить его к области видимости с помощью using enum LogStatus;


class Log 
{
    Log()
    {
        std::cout<<"Logger initialized"<<"\n";
    };
    static Log* logInstance;

    typedef struct logData
    {
        std::string message;
        std::time_t time;
        LogStatus status;
        void print()
        {
            std::cout<<std::ctime(&time)<<status<<" | "<<message <<"\n";
        }
    } dataPiece;
    std::vector<dataPiece> data;

public:
    Log(const Log&) = delete;
    void operator* (const Log&) = delete;

    static Log* Instance()
    {
        if(logInstance==nullptr)
            logInstance = new Log();
        return logInstance;
    }

    void message(LogStatus status, std::string text)
    {
        auto curTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        dataPiece message = {text, curTime, status};
        data.push_back(message);
    }
    void print() const
    {
        for(auto dataPiece: data)
            dataPiece.print();
    }

};

Log* Log::logInstance = nullptr;



int main(void) 
{
    Log *log = Log::Instance();
    log->message(LOG_NORMAL, "program loaded");
    log->message(LOG_WARNING, "program is few unnormal");
    log->message(LOG_ERROR, "error happens! help me!");
    log->print();

    std::cout<<"\nNow let's try to create another logger\n";
    Log *log2 = Log::Instance();
    log2->print();
    std::cout<<"\nOops, we can only get existed logger\n";

    //std::cout<<"a try to create by constructor failed\n";
    //Log *log3 = new Log;
    //Log log = Log();
    return 0;
}