cmake_minimum_required(VERSION 3.6)

project(main)
SET(CMAKE_BUILD_TYPE "Debug")

add_subdirectory(app app_build)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} app)
