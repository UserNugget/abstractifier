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
        src/util/object_pool.cpp
)

# Compile as static library for Windows
if (MSVC)
    set(COMPILER_OPTIONS /MT /GL /Ox)
    set(LINKER_OPTIONS /SUBSYSTEM:Windows /ENTRY:mainCRTStartup /LTCG /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)

    target_compile_options(abstractifier PUBLIC ${COMPILER_OPTIONS})
    target_link_options(abstractifier PUBLIC ${LINKER_OPTIONS})

    target_compile_options(glfw PUBLIC ${COMPILER_OPTIONS})
    target_link_options(glfw PUBLIC ${LINKER_OPTIONS})
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    set(COMPILER_OPTIONS -O3 -flto -ffunction-sections -fdata-sections)
    set(LINKER_OPTIONS -s -fuse-ld=lld -Wl,-strip-all,--gc-sections,--build-id=none)

    target_compile_options(abstractifier PUBLIC ${COMPILER_OPTIONS})
    target_link_options(abstractifier PUBLIC ${LINKER_OPTIONS})

    target_compile_options(glfw PUBLIC ${COMPILER_OPTIONS})
    target_link_options(glfw PUBLIC ${LINKER_OPTIONS})
endif()
