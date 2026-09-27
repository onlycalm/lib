cmake_minimum_required(VERSION 3.0)
project(Log)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)
include_directories(../Com)

aux_source_directory(. CUR_SOURCES)
aux_source_directory(../Com COM_SOURCES)

set(SRC_LIST ${CUR_SOURCES} ${COM_SOURCES})

add_executable(app ${SRC_LIST})
