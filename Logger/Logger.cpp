#include <cstdio>
#include <string>
#include <cstring>
#include <time.h>
#include <cassert>

#define ERR_LEVEL 0
#define WARN_LEVEL 1
#define INFO_LEVEL 2
#define DEBUG_LEVEL 3
 
class Logger {
    public:
    static Logger* GetInstance(int stdchannel);
    static Logger* GetInstance(const std::string& filepath);

    int GetSeverity() { return mSeverityLevel;}
    void SetSeverity(int level) { mSeverityLevel = level;}

    virtual void log(int serverityLevelOfMessage, const char* format,...) = 0;

    protected:
    int mSeverityLevel;
    Logger() {}
    Logger(const Logger& ) = delete;
    Logger& operator=(const Logger& ) = delete;
};
 
class LoggerFile : public Logger {
    friend Logger* Logger::GetInstance(int stdchannel);
    friend Logger* Logger::GetInstance(const std::string& filepath);
    public:
    void log(int serverityLevelOfMessage, const char* format,...) {
    //... https://www.cplusplus.com/reference/cstdio/vsprintf/
    //... https://man7.org/linux/man-pages/man2/gettimeofday.2.html
        char * str;
        char * serverityName;
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char s[64];
        str = (char*)format;
        assert(strftime(s, sizeof(s), "%c", &tm));
        //printf("[%s\n][%d][%s]", s,serverityLevelOfMessage,str);
        
        if (serverityLevelOfMessage <= mSeverityLevel) 
        {
            if (serverityLevelOfMessage == 0)
            {
                serverityName = (char*)"ERR";
            }
            if (serverityLevelOfMessage == 1)
            {
                serverityName = (char*)"WARN";
            }
            if (serverityLevelOfMessage == 2)
            {
                serverityName = (char*)"INFO";
            }
            if (serverityLevelOfMessage == 3)
            {
                serverityName = (char*)"DEBUG";
            }
            fprintf(mOutputHandle,"[%s][%s][%s]\n", s,serverityName,str);
            //fwrite(str,strlen(str)+1,1,mOutputHandle);
        }

        // if (mOutputHandle != NULL)
        // {
        //     fclose(mOutputHandle);
        // } 
    }
    protected:
    FILE* mOutputHandle;
    LoggerFile(const std::string& filePath) 
    {
        mOutputHandle = fopen(filePath.c_str(),"w+");
    }
    LoggerFile(FILE* alreadyHandle) 
    {
        mOutputHandle = alreadyHandle;
    }
    LoggerFile() {}
    LoggerFile(const LoggerFile& ) = delete;
    LoggerFile& operator=(const LoggerFile& ) = delete;
};
 
//extern FILE* stdout;
//extern FILE* stderr;
 
Logger* Logger::GetInstance(int stdchannel) {
    if (stdchannel == 0) { //logger for stdout
        return new LoggerFile(stdout);
    }
    else { // logger for stderr
        return new LoggerFile(stderr);
    }
}

Logger* Logger::GetInstance(const std::string& filePath) {
    return new LoggerFile(filePath);
}



int main() {
    
    int i = 3;
    int k = 5;
    int l = 7;
 // printf("sadfsad" ); //-> fprintf(stdout,"sadfsad");
 // fprintf(stdout,"sadfsad");
    Logger* logger = Logger::GetInstance(0);
    Logger* logger2 = Logger::GetInstance("E:/File_Documents/VS2019/FPT/Assg6.8_Virtual_TrieuPN1/logfile.log");
    Logger* logger3 = Logger::GetInstance("file.txt");
    logger->SetSeverity(INFO_LEVEL);
    logger2->SetSeverity(INFO_LEVEL);
    logger3->SetSeverity(INFO_LEVEL);
    logger->log(DEBUG_LEVEL,"Debug message ..... ");
    //logger2->log(INFO_LEVEL,"Info message ..... ",i,k,l);
    //logger3->log(INFO_LEVEL,"Infor Message......");
    // do { // logger to stdout
    //     Logger* logger = Logger::GetInstance(0);
        
    //     logger->SetSeverity(INFO_LEVEL);
        
    //     logger->log(WARN_LEVEL,"Hello world");
        
    //     logger->log(DEBUG_LEVEL,"Debug message .....");
    
    // }while(0);
 
    // do { // logger to stderr
    //     Logger* logger = Logger::GetInstance(1);
        
    //     logger->SetSeverity(INFO_LEVEL);
        
    //     logger->log(WARN_LEVEL,"Hello world");
        
    //     logger->log(DEBUG_LEVEL,"Debug message .....");
        
    // }while(0);
 
    // do { // logger to stderr
    //     Logger* logger = Logger::GetInstance("logdata.log");
        
    //     logger->SetSeverity(INFO_LEVEL);
        
    //     logger->log(WARN_LEVEL,"Hello world"); //[2021-06-04 22:20:56.567][WARN] Hello world
        
        
    //     logger->log(INFO_LEVEL,"Info message .....");
        

    //     logger->log(INFO_LEVEL,"Debug message %d - %d - %d.....", i,k,l);
    //     //[2021-06-04 22:20:56.567][INFO] Debug message 5 - 6 - 7
    // }while(0);
 
 
 // fprintf() -> file handle -> fopen
}
