#CMake version required >= 2.8
cmake_minimum_required (VERSION 2.8)

#project info
project(client_main)

#add header path
include_directories(../)

#execute obj
add_executable(client_main client_main.cpp)

#link lib
link_libraries(ssl crypto)

target_link_libraries(client_main -lssl -lcrypto)
