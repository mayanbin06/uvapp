cmake_minimum_required(VERSION 3.6)

project(app)

set(CMAKE_CXX_FLAG "-std=c++11")
set(ROOT ${PROJECT_SOURCE_DIR}/../)

include_directories(${ROOT})

# base dependence
add_subdirectory(${ROOT}/base base_build)

# libuv dependence
include_directories(${ROOT}/third_party/libuv/include)
add_subdirectory(${ROOT}/third_party/libuv libuv_build)

set(APP_SRC_LIST
  loop.h
  loop.cc
)

if (WIN32)
  list(APPEND definitions OS_WIN)
else ()
  list(APPEND definitions OS_POSIX)
endif ()


add_library(${PROJECT_NAME} SHARED ${APP_SRC_LIST})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${definitions})
target_link_libraries(${PROJECT_NAME} uv base)
