cmake_minimum_required(VERSION 3.11)

set(PROJECT_NAME "Renderer")
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext")
file(GLOB SOURCES 
    "src/*.cpp"
)
file(GLOB HEADERS 
    "src/include/*.h"
)
file(GLOB TEST_APP_SOURCES
    ${PROJECT_SOURCE_DIR}/test/*.cpp
)

add_definitions(-DSINGLE_PRECISION)
add_library(${PROJECT_NAME}
    STATIC
        ${SOURCES} ${HEADERS} ${TINY_OBJ_SOURCES}
)
target_include_directories(${PROJECT_NAME} 
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/include
)

# OpenGL
add_definitions(-DGLEW_STATIC)

add_subdirectory(${EXT_DIR}/glfw EXCLUDE_FROM_ALL)
add_subdirectory(${EXT_DIR}/glew-cmake EXCLUDE_FROM_ALL)
add_subdirectory(${EXT_DIR}/glm EXCLUDE_FROM_ALL)
add_subdirectory(${EXT_DIR}/tinyobjloader EXCLUDE_FROM_ALL)

target_link_libraries(${PROJECT_NAME}
    PUBLIC glfw
    PUBLIC libglew_static
    PUBLIC glm
    PUBLIC tinyobjloader
)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(${PROJECT_NAME}_test ${TEST_APP_SOURCES})
target_link_libraries(${PROJECT_NAME}_test
    PUBLIC
        ${PROJECT_NAME}
)