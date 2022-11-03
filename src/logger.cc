#include "logger.h"
#include <time.h>
#include <iostream>

// 获取日志的到单例，在类外实现静态成员不需要再加static
Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

Logger::Logger()
{
    // 启动专门的写日志线程
    std::thread writeLogTask([&]{
        // 获取当前的日期，然后取日志信息，写入相应的日志文件当中 a+(追加的方式)
        for (;;)
        {
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);

            char file_name[128];
            sprintf(file_name, "%d-%d-%d.txt", nowtm->tm_year + 1900, nowtm->tm_mon + 1, nowtm->tm_mday);
            FILE *pf = fopen(file_name, "a+");
            if (pf == nullptr)
            {
                std::cout << "logger file: " << file_name << "open error!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string msg = m_lckQue.Pop();

            char time_buf[128] = {0};
            // 给日志信息加上时间： 时：分：秒
            sprintf(time_buf, "%d:%d:%d => [%s]",   // 输出时把日志的级别也输出，方便查找日志文件中的error和info
                    nowtm->tm_hour, 
                    nowtm->tm_min, 
                    nowtm->tm_sec,
                    (m_loglevel == INFO ? "info" : "error"));
            msg.insert(0, time_buf);
            msg.append("\n");

            fputs(msg.c_str(), pf);
            fclose(pf);   // 这里写的保守，写一次，关闭一次，可以判断下对列不为空，可以一次性取出，然后全部写完再关
        }
       

    });
    // 设置分离线程，相当于守护线程，专门在后台写日志
    writeLogTask.detach();
}

// 设置日志的级别
void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level;
}
// 写日志，把日志信息写入到lockqueue缓冲区当中
void Logger::Log(std::string msg)
{
    m_lckQue.Push(msg);
}