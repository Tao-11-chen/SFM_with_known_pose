cmake_minimum_required(VERSION 2.8)

project(bundle_adjustment)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-O3 -std=c++11")

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(OpenCV 4 REQUIRED)
Find_Package(Eigen3 REQUIRED)
Find_Package(Ceres REQUIRED)

include_directories(${PROJECT_SOURCE_DIR} ${EIGEN3_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

add_library(bal_common common.cpp ProcessData.cpp)
add_executable(bundle_adjustment_ceres bundle_adjustment_ceres.cpp)
target_link_libraries(bundle_adjustment_ceres ${CERES_LIBRARIES} bal_common)

add_library(${PROJECT_NAME} ProcessData.cpp)
add_executable(ProcessData main.cpp)
target_link_libraries(ProcessData ${OpenCV_LIBS} ${PROJECT_NAME})

