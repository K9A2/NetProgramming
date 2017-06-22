cmake_minimum_required(VERSION 3.7)
project(select)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        README.txt
        select_cli.c
        select_server.c
        select_server_good.c)

add_executable(select ${SOURCE_FILES})