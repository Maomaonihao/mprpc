#pragma once 
#include "lockqueue.h"
#include <string>

#define LOG_INFO(logmsgformat, ...) \
    do \
    {  \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(INFO); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c); \
    } while(0) \

#define LOG_ERR(logmsgformat, ...) \
    do \
    {  \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(ERROR); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c); \
    } while(0) \

enum LogLevel
{
    INFO,  // 普通日志信息
    ERROR,  // 错误日志信息
    // 可以分得更细
};

// Mprpc框架提供的日志系统
class Logger  // 设为单例模式
{
public:
    // 获取日志的到单例
    static Logger& GetInstance();
    // 设置日志的级别
    void SetLogLevel(LogLevel level);
    // 写日志
    void Log(std::string msg);

private:
    int m_loglevel;   // 记录日志级别
    LockQueue<std::string> m_lckQue;   // 日志缓冲队列

    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
};

// 以下宏定义有问题，\ 后面不能有其它的符合和空格时才有效
// // 定义宏 LOG_INFO("xxx %d %s", 20, "xxxx")
// #define LOG_INFO(logmsgformat, ...) \    // 就和printf函数一样，支持可变参，用户写日志时，可以组织自己的日志风格
//     do \             // 为了防止日志出问题，一般都会组织一个do while
//     {  \
//         Logger &logger = Logger::GetInstance(); \
//         logger.SetLogLevel(INFO);  \
//         char c[1024] = {0};   \
//         snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \     // ##__VA_ARGS__代表可变参的参数列表
//         logger.Log(c);  \
//     }whiel({0}) \

// #define LOG_ERROR(logmsgformat, ...) \   // 就和printf函数一样，支持可变参，用户写日志时，可以组织自己的日志风格
//     do  \           // 为了防止日志出问题，一般都会组织一个do while
//     {  \
//         Logger &logger = Logger::GetInstance(); \ 
//         logger.SetLogLevel(ERROR);  \
//         char c[1024] = {0}; \  
//         snprintf(c, 1024, logmsgformat, ##__VA_ARGS__);   \   // ##__VA_ARGS__代表可变参的参数列表
//         logger.Log(c); \
//     }whiel({0}) \
// 定义宏 LOG_INFO("xxx %d %s", 20, "xxxx")

