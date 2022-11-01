#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"
#include <iostream>

int main(int argc, char **argv)
{
    // 整个程序启动以后，想使用mprpc框架来享受rpc,一定需要先调用框架的初始化函数(只初始化一次)
    MprpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");

    fixbug::LoginResponse response;
    stub.Login(nullptr, &request, &response, nullptr);   // RpcChannel->RpcChannel::callMethod  集中来做所有rpc方法调用的参数序列化和网络发送
    return 0;
}
