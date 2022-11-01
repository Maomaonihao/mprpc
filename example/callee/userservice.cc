#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include <iostream>
#include <string>

// using namespace fixbug;
/*
UserService原来是一个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
*/
class UserService : public fixbug::UserServiceRpc   // 使用在rpc服务的发布端（rpc服务提供者）
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd << std::endl;
    }

    // 重写基类UserService的虚函数，下面这些方法都是框架直接调用的
    /*
        1、caller -> Login(LoginRequest) -> muduo -> callee
        2、callee -> Login(LoginRequest) -> 交到下面重写的Login方法上
    */
    void Login(::google::protobuf::RpcController* controller,
                    const ::fixbug::LoginRequest* request,
                    ::fixbug::LoginResponse* response,
                    ::google::protobuf::Closure* done)
    {
        // 框架给业务上报了请求参数LoginRequest（远程请求通过LoginRequest这个参数暴露出来），应用数据获取相应数据做本地业务
        // protobuf的好处，直接把网络上的字节流反序列化为loginRequest对象
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name, pwd);  

        // 把响应想入response，至于怎么序列化，怎么传，都交给框架做了
        // 报错错误码，错误消息，返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        // 调用执行回调操作， 执行响应数据的序列化和网络发送（都由框架来完成）
        // Closure是一个抽象类，肯定不是指向Cloure对象的，这里肯定要定义一个继承自Closure的类，然后重写Run()方法
        // 当然这里也可以传一个匿名的函数对象，lambda表达式
        done->Run();

    }
};

int main(int argc, char **argv)   // argc, argv这些参数都是从配置文件而来
{
    // 调用框架的初始化操作  provider -i config.conf
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动一个rpc服务发布节点    Run以后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}
