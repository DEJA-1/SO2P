cmake_minimum_required(VERSION 3.28)
project(SO2P)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(dining_philosophers
        km/main.cpp
)

target_include_directories(dining_philosophers PRIVATE km)

find_package(Threads REQUIRED)
target_link_libraries(dining_philosophers PRIVATE Threads::Threads)
