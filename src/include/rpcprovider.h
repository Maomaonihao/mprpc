#pragma once
#include "google/protobuf/service.h"

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <functional>
#include <unordered_map>
#include <string>
#include <functional>
#include <google/protobuf/descriptor.h>
#include <unordered_map>

// 框架提供的专门服务发布rpc服务的网络对象类
class RpcProvider
{
public:
    // 这是有问题的，框架肯定不能依赖于具体某个业务，应该为所有的业务提供服务，应该站在抽象的角度考虑问题
    // void NotifyService(UserService *service){};

    // 这里是框架提供给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);

    // 启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();
    // 新的socket连接回调
   

private:
    // 组合了EventLoop(执行的是epoll)，这个变量需要在多个成员方法中使用，所以不能定义为局部变量，得定义为成员变量
    muduo::net::EventLoop m_eventLoop;

    struct ServiceInfo
    {
        google::protobuf::Service *m_service;    // 保存服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap;  // 保存服务方法
    };
    // 存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;

    // // 新的sockt连接回调
    // void OnConnection(const muduo::net::TcpConnection&);

    // // 已建立连接用户的读写事件回调
    // // muduo库如果发现有可读写的消息发生后，就会主动调用回调函数，通知上层
    // void OnMessage(const muduo::net::TcpConnection& , muduo::net::Buffer* , muduo::Timestamp);
    // 新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr& conn);
    // 已建立连接用户的读写事件回调
    void OnMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer*, muduo::Timestamp);
    // Closure回调操作，用于序列化rpc的响应和网络发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr&, google::protobuf::Message*);
};