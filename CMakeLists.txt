cmake_minimum_required(VERSION 3.15)

option(USE_PROXY "Enable proxy for FetchContent for file download" ON)
if (USE_PROXY)
    message(STATUS "Proxy is enabled")
    set(ENV{http_proxy} "http://127.0.0.1:7890")
    set(ENV{https_proxy} "http://127.0.0.1:7890")
    set(CMAKE_TLS_VERIFY OFF)
    set(CMAKE_DOWNLOAD_AGENT "curl")
endif ()

# 项目信息
project(my-video-app VERSION 1.0 LANGUAGES C CXX)

# 平台判断并设置默认编译器 仅在未手动指定时生效
if (NOT DEFINED CMAKE_C_COMPILER AND NOT DEFINED CMAKE_CXX_COMPILER)
    if (APPLE)
        message(STATUS "Detected Apple platform")
        set(CMAKE_C_COMPILER /usr/bin/clang CACHE STRING "C compiler" FORCE)
        set(CMAKE_CXX_COMPILER /usr/bin/clang++ CACHE STRING "C++ compiler" FORCE)
    elseif (UNIX)
        message(STATUS "Detected Linux platform")
        set(CMAKE_C_COMPILER /usr/bin/gcc CACHE STRING "C compiler" FORCE)
        set(CMAKE_CXX_COMPILER /usr/bin/g++ CACHE STRING "C++ compiler" FORCE)
    else ()
        message(FATAL_ERROR "Unsupported platform")
    endif ()
else ()
    message(STATUS "Using user-defined compiler: ${CMAKE_C_COMPILER}, ${CMAKE_CXX_COMPILER}")
endif ()

# c/cpp标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror)
# compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 依赖的三方库
include(${CMAKE_SOURCE_DIR}/lib/spdlog.cmake)
include(${CMAKE_SOURCE_DIR}/lib/glfw.cmake)
include(${CMAKE_SOURCE_DIR}/lib/glad.cmake)

# 开始编译自己的项目
file(GLOB_RECURSE srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

# 生成库文件 添加头文件的目的仅仅是为了让IDEA识别到 新建文件的时候添加索引
add_executable(${PROJECT_NAME} ${srcs})

# lib
target_link_libraries(${PROJECT_NAME}
        PUBLIC spdlog
        PUBLIC glfw
        PUBLIC glad
)

if (LINUX)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl GL GLU X11)
elseif (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGl)
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
else ()
    message(FATAL_ERROR "I DONT DETECT THE OS!!!")
endif ()