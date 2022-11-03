#pragma once
#include <queue>
#include <thread>
#include <mutex>    // 对应linux中的pthread_mutex_t 
#include <condition_variable>   // 对应linux中的pthread_condition_t

// 异步写日志的日志队列
template<typename T>
class LockQueue
{
public:
    // 入队和出队用的是同一把锁

    // 多个worker线程都会写日志queue
    void Push(const T &data)  // push是由框架的工作线程来调用
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(data);
        m_convariable.notify_one();
    }

    // 一个线程读日志queue，写日志文件
    T Pop()  // 专门的往磁盘写日志的线程来调用
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty())  // 这里用while防止线程的虚假唤醒
        {
            // 日志队列为空，现场进入wait状态
            m_convariable.wait(lock);  // 进入到wait状态，并把锁释放掉，这样负责入队的线程就可以往队列中写信息了
        }

        // 收到notify之后，一看queue不为空，退出循环
        T data = m_queue.front();
        m_queue.pop();
        return data;
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_convariable;
};