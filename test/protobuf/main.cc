#include "test.pb.h"
#include <iostream>
#include <string>

using namespace fixbug;   // 在实际开发时是不允许直接这样使用的，因为可能引起名字的冲突

int main(){
    // LoginResponse rsp;
    // ResultCode *rc = rsp.mutable_result();   // 如果一个对象的成员变量也是一个对象，那么一定要先获取muttable
    // rc->set_errcode(1);
    // rc->set_errmsg("登录处理失败了");

    GetFriendListsResponse rsp;
    ResultCode *rc = rsp.mutable_result();
    rc->set_errcode(0);

    User *user1 = rsp.add_friend_list();
    user1->set_name("zhangsan");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    User *user2 = rsp.add_friend_list();
    user2->set_name("zhangsan");
    user2->set_age(20);
    user2->set_sex(User::MAN);

    std::cout << rsp.friend_list_size() << std::endl;


    return 0;
}

int main1()
{
    // 封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    // 对象数据序列化=》 char *
    std::string send_str;
    if(req.SerializeToString(&send_str)){  // 如果序列化成功，则放入到send_str
        std::cout << send_str << std::endl;
    }

    // 从send_str反序列化一个login请求对象
    LoginRequest reqB;
    if (reqB.ParseFromString(send_str)){
        std::cout << reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    }

    return 0;
}