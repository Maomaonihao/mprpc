#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include <iostream>
#include <string>
#include <vector>
#include "logger.h"

class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendList(uint32_t userid)
    {
        std::cout << "do GetFriendList service! userid: " << userid <<  std::endl;
        std::vector<std::string> vec;
        vec.push_back("gao yang");
        vec.push_back("mao mao");
        vec.push_back("yang hua");

        return vec;
    }

    // 重写基类方法
    void GetFriendList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendListRequest* request,
                       ::fixbug::GetFriendListResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t userid = request->userid();
        std::vector<std::string> friendsList = GetFriendList(userid);
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for (std::string &name : friendsList)
        {
            std::string *p = response->add_friends();
            *p = name;
        }
        done->Run();
    }
};


int main(int argc, char **argv)   // argc, argv这些参数都是从配置文件而来
{
    LOG_INFO("first log message!");
    LOG_ERR("%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);  
    // LOG_ERR("dddddddddddd");
    // 调用框架的初始化操作  provider -i config.conf
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    // 启动一个rpc服务发布节点    Run以后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}