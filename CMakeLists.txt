cmake_minimum_required(VERSION 3.15)
project(LearnRayTracing)

set(CMAKE_CXX_STANDARD 14)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/thirdparty")
include_directories("${PROJECT_SOURCE_DIR}/thirdparty/eigen/Eigen")
include_directories("${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include")

add_subdirectory(src)

add_executable(main main.cpp)
target_link_libraries(main Utils Geometry RTCore)

add_executable(testMC testMC.cpp)
