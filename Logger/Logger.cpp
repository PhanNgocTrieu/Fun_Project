#include <cstdio>
#include <string>
#include <cstring>
#include <time.h>
#include <cassert>
#include <stdarg.h>
#include <sys/timeb.h>

#define ERR_LEVEL 0
#define WARN_LEVEL 1
#define INFO_LEVEL 2
#define DEBUG_LEVEL 3

class Logger
{
public:
    static Logger *GetInstance(int stdchannel);
    static Logger *GetInstance(const std::string &filepath);

    int GetSeverity() { return mSeverityLevel; }
    void SetSeverity(int level) { mSeverityLevel = level; }

    virtual void log(int serverityLevelOfMessage, const char *format, ...) = 0;

protected:
    int mSeverityLevel;
    Logger() {}
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
};

class LoggerFile : public Logger
{
    friend Logger *Logger::GetInstance(int stdchannel);
    friend Logger *Logger::GetInstance(const std::string &filepath);

public:
    void log(int serverityLevelOfMessage, const char *format, ...)
    {
        //... https://www.cplusplus.com/reference/cstdio/vsprintf/
        //... https://man7.org/linux/man-pages/man2/gettimeofday.2.html
        time_t t = time(NULL); 
        struct tm tm = *localtime(&t);
        char s[256];
        strftime(s, 256, "%c", &tm);

    
        va_list arguments;
        int len = _vscprintf(format,arguments);
        char* buffer = new char[len];
        /* Initializing arguments to store all values after num */
        va_start(arguments, format);
        vsprintf(buffer, format, arguments);
        //perror (buffer);
        va_end(arguments);
        char *serverityName;


        if (serverityLevelOfMessage < 0 || serverityLevelOfMessage > 3)
        {
            std::string serverityLevel[] = { "ERR", "WARN", "INFOR", "DEBUG"};

            fprintf(mOutputHandle,"[%s][%s][%s]\n", s,serverityLevel[serverityLevelOfMessage],buffer);
        }


    }

protected:
    FILE *mOutputHandle;
    LoggerFile(const std::string &filePath)
    {
        mOutputHandle = fopen(filePath.c_str(), "w+");
    }
    LoggerFile(FILE *alreadyHandle)
    {
        mOutputHandle = alreadyHandle;
    }
    LoggerFile() {}
    LoggerFile(const LoggerFile &) = delete;
    LoggerFile &operator=(const LoggerFile &) = delete;
};

//extern FILE* stdout;
//extern FILE* stderr;

Logger *Logger::GetInstance(int stdchannel)
{
    if (stdchannel == 0)
    { //logger for stdout
        return new LoggerFile(stdout);
    }
    else
    { // logger for stderr
        return new LoggerFile(stderr);
    }
}

Logger *Logger::GetInstance(const std::string &filePath)
{
    return new LoggerFile(filePath);
}

int main()
{

    int i = 3;
    int k = 5;
    int l = 7;

    do { // logger to stdout
        Logger* logger = Logger::GetInstance(0);

        logger->SetSeverity(INFO_LEVEL);

        logger->log(WARN_LEVEL,"Hello world");

        logger->log(DEBUG_LEVEL,"Debug message .....");

        logger->log(INFO_LEVEL,"Infor message .....");

    }while(0);

    do { // logger to stderr
        Logger* logger = Logger::GetInstance(1);

        logger->SetSeverity(INFO_LEVEL);

        logger->log(WARN_LEVEL,"Hello world");

        logger->log(DEBUG_LEVEL,"Debug message .....");

        logger->log(ERR_LEVEL,"Error message .....");

    }while(0);

    do { // logger to stderr
        Logger* logger = Logger::GetInstance("logdata.log");

        logger->SetSeverity(INFO_LEVEL);

        logger->log(WARN_LEVEL,"Hello world"); //[2021-06-04 22:20:56.567][WARN] Hello world

        logger->log(INFO_LEVEL,"Infor message .....");

        logger->log(INFO_LEVEL,"Infor message %d - %d - %d.....", i,k,l);

        logger->log(DEBUG_LEVEL,"Debug message %d - %d - %d.....", i,k,l);

        logger->log(ERR_LEVEL,"Error message %d - %d - %d.....", i,k,l);
        //[2021-06-04 22:20:56.567][INFO] Debug message 5 - 6 - 7
    }while(0);


    return 0;
    // fprintf() -> file handle -> fopen
}
