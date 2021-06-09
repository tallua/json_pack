cmake_minimum_required(VERSION 3.15)

project(jpack_test)

add_executable(jpack_test test/main.cpp test/suite/sample_suite.cpp)

set_property(TARGET jpack_test PROPERTY CXX_STANDARD 11)
set_property(TARGET jpack_test PROPERTY CXX_STANDARD_REQUIRED ON)

# third party
add_subdirectory(third_party/googletest)

# include & link
include_directories(third_party/googletest/googletest/include)
include_directories(third_party/nlohmann_json/single_include)
include_directories(include)
include_directories(test/include)

target_link_libraries(jpack_test gtest)

link_directories(${CMAKE_CURRENT_BINARY_DIR})