cmake_minimum_required(VERSION 3.0)

add_library(ibsl INTERFACE)
target_include_directories(ibsl INTERFACE include)

add_executable(ibsl-test test/hello.cpp)
target_link_libraries(ibsl-test ibsl)
