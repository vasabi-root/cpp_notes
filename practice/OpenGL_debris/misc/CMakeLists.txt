cmake_minimum_required(VERSION 3.12)
project(tgaimage)

# set(CMAKE_C_COMPILER gcc.exe)
# set(CMAKE_CXX_COMPILER g++.exe)

set(SOURCES tgaimage.cpp model.cpp)
set(HEADERS tgaimage.h model.h geometry.h)

add_library(tgaimage ${SOURCES} ${HEADERS})
target_include_directories(tgaimage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})