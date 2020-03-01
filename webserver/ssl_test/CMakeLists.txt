#CMake version required >= 2.8
cmake_minimum_required (VERSION 2.8)

#project info
project(server_main)

#execute obj
add_executable(server_main server_main.cpp)

#link lib
link_libraries(ssl crypto)

target_link_libraries(server_main -lssl -lcrypto)
