cmake_minimum_required(VERSION 3.0)
project(BrgDn)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 头文件搜索路径：本目录(BrgDn.h)、上级目录(Sta.h)、Com(Typ.h/Er.h)、Log(Log.h)。
include_directories(.)
include_directories(..)
include_directories(../../Com)
include_directories(../../Log)

set(SRC_LIST main.c BrgDn.c ../Sta.c)

add_executable(app ${SRC_LIST})
