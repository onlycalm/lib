cmake_minimum_required(VERSION 3.0)
project(sta)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)
include_directories(../com)

set(SRC_LIST main.c Sta.c StaImpl.c)

add_executable(app ${SRC_LIST})
