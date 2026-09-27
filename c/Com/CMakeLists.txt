cmake_minimum_required(VERSION 3.0)
project(Com)

include_directories(.)
include_directories(../Log)

# Com.c 内部使用了 log 模块的日志宏，故需一并编译 Log.c 提供日志实现
add_library(Com STATIC Com.c ../Log/Log.c)
