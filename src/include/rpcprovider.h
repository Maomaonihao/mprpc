#pragma once
#include "google/protobuf/service.h"

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

};