cmake_minimum_required(VERSION 3.0)
project(litchi)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)
include_directories(../com)

aux_source_directory(. CUR_SOURCES)
aux_source_directory(../com COM_SOURCES)

set(SRC_LIST ${CUR_SOURCES} ${COM_SOURCES})

add_executable(app ${SRC_LIST})
