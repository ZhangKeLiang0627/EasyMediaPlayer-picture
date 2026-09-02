cmake_minimum_required(VERSION 3.10)
project(eMP_picture)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# 编译选项
# Default to C99
set(CMAKE_C_STANDARD 99)
# Default to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 系统信息打印
message(STATUS "System : ${CMAKE_SYSTEM_NAME}")
message(STATUS "Target system processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Host architecture: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")

# 基础编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g0 -Wall -Wno-unused-function -Wno-unused-variable -Wno-return-type -Wno-sign-compare -Wno-memset-transposed-args")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")

set(COMMON_LIBRARIES
    pthread
    stdc++fs
    stdc++
    m
)

# 交叉链接的平台/图像库（与仓库 Makefile 的 LDFLAGS 保持一致；
# 放在列表末尾以兼容静态库链接顺序）
set(PLATFORM_LIBRARIES
    cdx_base
    ncurses
    jpeg
    png
    z
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

# 检查本地/交叉编译（交叉编译时由 cmake/build_for_t113s3.cmake 把
# CMAKE_HOST_SYSTEM_PROCESSOR 置为 arm，自动落入交叉分支）
find_package(PkgConfig REQUIRED)
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
    message(STATUS "Local-compilation (PC, SDL2)")

    set(LV_DRIVERS_USE_WAYLAND ON CACHE BOOL "Enable Wayland for local build")

    # 添加本地编译宏定义
    add_definitions(-DLV_USE_DRAW_SDL=0 -DUSE_SDL=1 -DUSE_MOUSE=1)

    # 使用pkg-config查找SDL2
    find_package(SDL2 REQUIRED)
    list(APPEND COMMON_LIBRARIES ${SDL2_LIBRARIES})

    # 添加Freetype头文件路径
    include_directories(/usr/include/freetype2)

    add_definitions(-DPROJECT_PATH="${PROJECT_SOURCE_DIR}")
    add_definitions(-DRESOURCES_PATH="${PROJECT_SOURCE_DIR}")
else()
    # 交叉编译（T113-S3）：驱动/显示配置由仓库自带 libs/lv_conf.h 与
    # libs/lv_drv_conf.h 决定，与 Makefile 构建保持完全一致，不额外注入宏
    message(STATUS "Cross-compilation (T113-S3)")

    add_definitions(-DPROJECT_PATH="${PROJECT_SOURCE_DIR}")
    add_definitions(-DRESOURCES_PATH="${PROJECT_SOURCE_DIR}")
endif()

# 查找源文件
file(GLOB_RECURSE C_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c"
)
file(GLOB_RECURSE CXX_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
)
set(MAIN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

set(ALL_SRCS
    ${C_SRCS}
    ${CXX_SRCS}
    ${MAIN_SRC}
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${ALL_SRCS})

# 添加子CMakeLists.txt（仓库自带 vendored 库）
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/lvgl)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/lv_drivers)

# 链接库
target_link_libraries(${PROJECT_NAME}
    lvgl
    lvgl::drivers
    freetype
    ${COMMON_LIBRARIES}
    ${PLATFORM_LIBRARIES}
)
