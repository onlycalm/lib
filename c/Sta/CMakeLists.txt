cmake_minimum_required(VERSION 3.0)
project(Sta)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)
include_directories(../Com)

set(SRC_LIST Main.c Sta.c StaImpl.c)

add_executable(app ${SRC_LIST})
