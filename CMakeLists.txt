cmake_minimum_required(VERSION 3.28)
project(abstractifier)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(lib/glfw/include)
include_directories(lib/glad/include)
include_directories(lib/glad/include)
include_directories(lib/stb)
include_directories(include)

link_libraries(OpenGL::GL Threads::Threads glfw)
add_executable(abstractifier
        lib/stb/stb_image.h
        lib/glad/src/gl.c
        src/launcher.cpp
        src/game.cpp
        src/math/vectors.cpp
        src/client/input/input.cpp
        src/client/texture/texture_manager.cpp
        src/client/draw/shader.cpp
        src/client/draw/window.cpp
        src/client/draw/renderer.cpp
        src/client/draw/draw_builder.cpp
        src/client/entity/client_entity.cpp
        src/client/world/client_world.cpp
        src/world/world.cpp
        src/world/entity.cpp
        src/world/bullet.cpp
        src/world/enemy.cpp
        src/util/lib.cpp
)

# Compile as static library for Windows
if (MSVC)
    target_compile_options(abstractifier PUBLIC /Wall /MTd)
    target_link_options(abstractifier PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)

    target_compile_options(glfw PUBLIC /Wall /MTd)
    target_link_options(glfw PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
endif()
