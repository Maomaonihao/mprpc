# aux_source_directory(. SRC_LIST)  # 将当前路径下的所有源文件名字放入到SRC_LIST
set(SRC_LIST 
    mprpcapplication.cc 
    mprpcconfig.cc 
    rpcheader.pb.cc 
    rpcprovider.cc 
    mprpcchannel.cc 
    mprpccontroller.cc
    logger.cc
    zookeeperutil.cc)
    
add_library(mprpc ${SRC_LIST})  # 生成动态库（SHARED），这里需要和muduo库一样，编译为静态库，不然使用的时候会有问题
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)
