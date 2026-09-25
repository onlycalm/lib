cmake_minimum_required(VERSION 3.0)
project(com)

include_directories(.)
include_directories(../log)

# com.c 内部使用了 log 模块的日志宏，故需一并编译 log.c 提供日志实现
add_library(com STATIC com.c ../log/log.c)
