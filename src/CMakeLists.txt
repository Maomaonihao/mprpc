aux_source_directory(. SRC_LIST)  # 将当前路径下的所有源文件名字放入到SRC_LIST
add_library(mprpc SHARED ${SRC_LIST})  # 生成动态库（SHARED）
