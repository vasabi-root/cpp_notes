cmake_minimum_required(VERSION 3.12)
project(OpenGL_debris)

# set(CMAKE_C_COMPILER gcc.exe)
# set(CMAKE_CXX_COMPILER g++.exe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O0 -g")
file(GLOB main CONFIGURE_DEPENDS "*.h" "*.cpp")

add_executable(main ${main} )

add_subdirectory(misc)
target_link_libraries(main PUBLIC tgaimage)
# target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/misc)