cmake_minimum_required(VERSION 2.8)

if(APPLE)
set(CMAKE_CXX_FLAGS "-std=c++14 -fexceptions -m64")
else(APPLE)
set(CMAKE_CXX_FLAGS "-std=c++14 -fexceptions -m64 -static")
endif(APPLE)

#set(CMAKE_CXX_FLAGS "-std=gnu++14 -fno-ms-compatibility -fexceptions -m64")
set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")

# ビルドするためにインクルードするヘッダーファイルの場所
INCLUDE_DIRECTORIES(./src ./src/common1)

LINK_DIRECTORIES(~/lib)
include_directories(~/include)

add_subdirectory(./src/common1)
add_subdirectory(./src)

add_executable(construct.out construct_main.cpp)
target_link_libraries(construct.out common1 tst)
add_executable(search.out search_main.cpp)
target_link_libraries(search.out common1 tst)