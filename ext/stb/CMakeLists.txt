cmake_minimum_required(VERSION 3.2)
project(stb)

set(stb_source
  ${CMAKE_CURRENT_SOURCE_DIR}/stb_image_write.h
  ${CMAKE_CURRENT_SOURCE_DIR}/stb_image_write.cpp
)

add_library(stb ${stb_source})
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
