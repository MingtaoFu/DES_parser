cmake_minimum_required(VERSION 3.7)
project(DES_parser)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.c tables.h gui.c)

cmake_minimum_required(VERSION 3.5)
project(DES_parser)

set(CMAKE_CXX_STANDARD 11)

find_library(pthread /usr/lib)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
message(staus "GTK3_INCLUDE_DIRS = ${GTK3_INCLUDE_DIRS}")
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(DES_parser ${SOURCE_FILES})
target_link_libraries(DES_parser pthread)
target_link_libraries(DES_parser ${GTK3_LIBRARIES})
