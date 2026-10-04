cmake_minimum_required(VERSION 3.0)
project(DevDtc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 头文件搜索路径：本目录(DevDtc.h)、上级目录(Dtc.h)、Com(Typ.h/Er.h)、Log(Log.h)。
include_directories(.)
include_directories(..)
include_directories(../../Com)
include_directories(../../Log)

set(SRC_LIST main.c DevDtc.c ../Dtc.c)

add_executable(app ${SRC_LIST})
