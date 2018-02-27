cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++14 -fexceptions -m64")
set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")

# ビルドするためにインクルードするヘッダーファイルの場所
#INCLUDE_DIRECTORIES(./tst ./esp ./xbw ./qxbw ./common1 ./common2 ./tstesp ./xbwesp)
#FILE(GLOB SOURCES tst/*.cpp esp/*.cpp xbw/*.cpp add/*.cpp)
#FILE(GLOB SOURCES *.cpp *.h)
INCLUDE_DIRECTORIES(./common1)
FILE(GLOB SOURCES *.cpp)
add_library(tst STATIC ${SOURCES})
#add_library(tst STATIC base_node.h rpatricia_tree.cpp truncated_suffix_tree.cpp tst_encode.cpp tst_file_manager.h)
