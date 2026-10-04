cmake_minimum_required(VERSION 3.0)
project(NetRx)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 头文件搜索路径：本目录(NetRx.h)、上级目录(RgQue.h)、Com(Typ.h/Er.h)、Log(Log.h)。
include_directories(.)
include_directories(..)
include_directories(../../Com)
include_directories(../../Log)

set(SRC_LIST main.c NetRx.c ../RgQue.c)

add_executable(app ${SRC_LIST})
