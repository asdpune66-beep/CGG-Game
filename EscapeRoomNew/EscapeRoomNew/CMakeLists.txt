cmake_minimum_required(VERSION 3.16)

project(EscapeRoomProtocolBlackout LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ============================================================
# GLFW PATHS
# ============================================================

set(GLFW_INCLUDE_DIR
    "${CMAKE_SOURCE_DIR}/third_party/glfw/include"
)

set(GLFW_LIBRARY
    "${CMAKE_SOURCE_DIR}/third_party/lib-mingw-w64/libglfw3.a"
)


# ============================================================
# OpenGL
# ============================================================

find_package(OpenGL REQUIRED)


# ============================================================
# GAME EXECUTABLE
# ============================================================

add_executable(EscapeRoom

    src/main.cpp

    src/core/Game.cpp
    src/core/Game.h

    src/graphics/Renderer.cpp
    src/graphics/Renderer.h
)


# ============================================================
# INCLUDE DIRECTORIES
# ============================================================

target_include_directories(EscapeRoom
    PRIVATE
        "${GLFW_INCLUDE_DIR}"
)


# ============================================================
# LIBRARIES
# ============================================================

target_link_libraries(EscapeRoom
    PRIVATE

        "${GLFW_LIBRARY}"

        OpenGL::GL

        gdi32
        user32
        shell32
        ws2_32
)