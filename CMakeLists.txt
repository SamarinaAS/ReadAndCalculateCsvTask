cmake_minimum_required(VERSION 3.16)

project(calculate_csv)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(include)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

add_executable(main src/main.cpp src/calculate_csv.cpp)
add_executable(tests src/tests.cpp)

target_link_libraries(tests ${GTEST_LIBRARIES} Threads::Threads)

