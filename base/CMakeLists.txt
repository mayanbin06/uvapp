cmake_minimum_required(VERSION 3.6)

project(base)

# set inlucde dir to the root project path.
#include_directories(..)

SET(base_sources
  macros.h
  threading/thread_local.h
  threading/thread_local_storage.h
  threading/thread_local_storage.cc
)

if (WIN32)
  list(APPEND base_defines OS_WIN)
  list(APPEND base_sources
       win/windows_types.h
       win/concurrencysal.h
       threading/thread_local_storage_win.cc
     )
else ()
  list(APPEND base_defines OS_POSIX)
  list(APPEND base_sources
       threading/thread_local_storage_posix.cc
     )
endif ()

#message(${base_sources})
#message(${base_defines})
add_library(${PROJECT_NAME} SHARED ${base_sources})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${base_defines})
#target_compile_options({$PROJECT_NAME} PRIVATE ${base_cflags})
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/..>
) 
