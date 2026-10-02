cmake_minimum_required(VERSION 3.0)
project(Sta)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)
include_directories(../Com)
include_directories(../Log)

set(SRC_LIST main.c Sta.c DevSta.c)

add_executable(app ${SRC_LIST})
