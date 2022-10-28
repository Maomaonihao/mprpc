#include "mprpcconfig.h"
#include <iostream>
#include <string>

// 负责解析加载配置文件
void MprpcConfig::LoadConfigFile(const char *config_file)
{
    FILE *pf = fopen(config_file, "r");
    if (nullptr == pf)
    {
        std::cout << config_file<< " is not exit!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 1. 注释  2.正确的配置项 3.去掉多余的空格
    while (!feof(pf)){   // 一行一行读
        char buf[512] = {0};
        fgets(buf, 512, pf);

        // 去掉字符串前面多余的空格，其他语言中有trim()函数可以去掉，但是C++中没有，需要自己做
        std::string read_buf(buf);  // string的构造函数的中的一种
        Trim(read_buf);
        // 判断注释  或 空行
        if (read_buf[0] == '#' || read_buf.empty())
        {
            continue;  // 读下一行
        }


        // 解析配置项
        int idx = read_buf.find('=');
        if (idx == -1){
            // 配置不合法
            continue;
        }

        std::string key;
        std::string value;
        key = read_buf.substr(0, idx);
        Trim(key);
        
        int endix = read_buf.find('\n', idx);
        value = read_buf.substr(idx + 1, endix - idx - 1);
        
        // value = read_buf.substr(idx + 1, read_buf.size() - idx - 1);
        Trim(value);
        m_configMap.insert({key, value});
    }

}
// 查询配置项信息
std::string MprpcConfig::Load(const std::string &key)
{
    // 一定不要用[]，m_configMap[key]。如果key不存在，会给map中增加
    auto it = m_configMap.find(key);
    if (it == m_configMap.end())
    {
        return "";
    }
    return it->second;
}

void MprpcConfig::Trim(std::string &read_buf)
{
    int idx = read_buf.find_first_not_of(' ');
    if (idx != -1){
        // 说明字符串前面有空格
        read_buf = read_buf.substr(idx, read_buf.size() - idx);
    }
    // 去掉字符串后面多余的空格 (写框架要健壮一点，要能把大部分情况都考虑到)
    idx = read_buf.find_last_not_of(' ');
    if (idx != -1){
        // 说明字符串后面有空格
        read_buf = read_buf.substr(0, idx + 1);
    }
}