cmake_minimum_required(VERSION 3.11)

set(PROJECT_NAME "Renderer")
project(${PROJECT_NAME} CXX)

option(BUILD_TESTS "Build test programs" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext")
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    # custom widgets
    ${PROJECT_SOURCE_DIR}/src/imgui/*.cpp
)
file(GLOB HEADERS
    ${PROJECT_SOURCE_DIR}/src/include/*.h
    # custom widgets
    ${PROJECT_SOURCE_DIR}/src/include/imgui/*.h
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
add_subdirectory(${EXT_DIR}/stb EXCLUDE_FROM_ALL)

# Manually add imgui
add_library(imgui STATIC
    ${EXT_DIR}/imgui/imgui.cpp
    ${EXT_DIR}/imgui/imgui_draw.cpp
    ${EXT_DIR}/imgui/imgui_widgets.cpp
    ${EXT_DIR}/imgui/imgui_demo.cpp
    ${EXT_DIR}/imgui/imgui_tables.cpp
)
# add windows, linux and mac specific files
# use glfw as backend
file(GLOB IMGUI_BACKEND_SOURCES
    ${EXT_DIR}/imgui/backends/imgui_impl_glfw.*
    ${EXT_DIR}/imgui/backends/imgui_impl_opengl3.*
    ${EXT_DIR}/imgui/backends/imgui_impl_win32.*
)
target_sources(imgui 
    PRIVATE
        ${IMGUI_BACKEND_SOURCES}
)
target_include_directories(imgui
    PUBLIC
        ${EXT_DIR}/imgui
        ${EXT_DIR}/imgui/backends
)
target_link_libraries(imgui
    PRIVATE
        glfw
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC glfw
    PUBLIC libglew_static
    PUBLIC glm
    PUBLIC tinyobjloader
    PUBLIC stb
    PUBLIC imgui
)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(BUILD_TESTS)
#add all test apps
add_executable(gui_test_app ${PROJECT_SOURCE_DIR}/test/gui_example/main.cpp)
target_link_libraries(gui_test_app ${PROJECT_NAME})

add_executable(minimal_app ${PROJECT_SOURCE_DIR}/test/minimal_example/main.cpp)
target_link_libraries(minimal_app ${PROJECT_NAME})
set_property(TARGET minimal_app PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/test/minimal_example")

endif()