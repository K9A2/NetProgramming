cmake_minimum_required(VERSION 3.7)
project(pcap_libnet)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-lpcap")
set(SOURCE_FILES pcap.c dns.c payload.c)
add_executable(pcap_libnet ${SOURCE_FILES})
set(CMAKE_VERBOSE_MAKEFILE ON)