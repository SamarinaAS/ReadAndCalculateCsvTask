cmake_minimum_required(VERSION 3.1)

project(calculate_csv)
option(BUILD_TESTS "flag for build tests" FALSE)
if (BUILD_TESTS)
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
endif()

include_directories(include)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

add_executable(main src/main.cpp src/calculate_csv.cpp)

if (BUILD_TESTS)
    add_executable(tests src/tests.cpp src/calculate_csv.cpp)

    target_link_libraries(tests ${GTEST_LIBRARIES} Threads::Threads)
endif()
