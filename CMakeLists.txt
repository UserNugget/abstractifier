cmake_minimum_required(VERSION 3.28)
project(abstractifier)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(lib/glfw/include)
include_directories(lib/glad/include)
include_directories(lib/stb)
include_directories(include)

link_libraries(OpenGL::GL glfw)
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
        src/client/draw/font.cpp
        src/client/hud/hud.cpp
        src/client/menu/menu.cpp
        src/client/menu/element/label.cpp
        src/client/menu/element/button.cpp
        src/client/menu/element/element.cpp
        src/client/menu/type/main_menu.cpp
        src/client/menu/type/pause_menu.cpp
        src/client/menu/overlay/overlay.cpp
)

if (MSVC)
    # Compile as static library for Windows
    set(COMPILER_OPTIONS /MT /Gw /GL /Ox)
    set(LINKER_OPTIONS /SUBSYSTEM:Windows /ENTRY:mainCRTStartup /LTCG /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    set(ALL_OPTIONS -flto=thin -fvisibility=hidden)
    set(COMPILER_OPTIONS ${ALL_OPTIONS} -O3 -ffast-math)
    set(LINKER_OPTIONS ${ALL_OPTIONS} -fuse-ld=lld -Wl,--strip-all,--gc-sections)
endif()

target_compile_options(abstractifier PUBLIC ${COMPILER_OPTIONS})
target_link_options(abstractifier PUBLIC ${LINKER_OPTIONS})

target_compile_options(glfw PUBLIC ${COMPILER_OPTIONS})
target_link_options(glfw PUBLIC ${LINKER_OPTIONS})

# Strip GLFW (reduce executable size by about 150kb)
# TODO: MSVC crashes when trying to 'allocate' 0 bytes {:
file(WRITE lib/glfw/src/mappings.h "const char* _glfwDefaultMappings[] = { \"78696e70757401000000000000000000,XInput Gamepad (GLFW),platform:Windows,a:b0,b:b1,x:b2,y:b3,leftshoulder:b4,rightshoulder:b5,back:b6,start:b7,leftstick:b8,rightstick:b9,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:a4,righttrigger:a5,dpup:h0.1,dpright:h0.2,dpdown:h0.4,dpleft:h0.8,\" };")
file(WRITE lib/glfw/src/vulkan.c "
void _glfwTerminateVulkan() { }
const char* _glfwGetVulkanResultString(int a) { return \"\"; }
")
file(WRITE lib/glfw/src/win32_joystick.c "
void _glfwDetectJoystickConnectionWin32() { }
void _glfwDetectJoystickDisconnectionWin32() { }
int _glfwInitJoysticksWin32() { return 0; }
void _glfwTerminateJoysticksWin32() { }
int _glfwPollJoystickWin32(void* a, int b) { return 0; }
const char* _glfwGetMappingNameWin32() { return \"\"; }
void _glfwUpdateGamepadGUIDWin32(char* a) { }
")
file(WRITE lib/glfw/src/linux_joystick.c "
void _glfwDetectJoystickConnectionLinux() { }
int _glfwInitJoysticksLinux() { return 0; }
void _glfwTerminateJoysticksLinux() { }
int _glfwPollJoystickLinux(void* a, int b) { return 0; }
const char* _glfwGetMappingNameLinux() { return \"\"; }
void _glfwUpdateGamepadGUIDLinux(char* a) { }
")
file(WRITE lib/glfw/src/osmesa_context.c "
int _glfwInitOSMesa() { return 0; }
void _glfwTerminateOSMesa() { }
int _glfwCreateContextOSMesa(void* a, const void* b, const void* c) { return 0; }
")
# TODO: required for wayland, remove in the future™
file(WRITE lib/glfw/src/egl_context.c "
int _glfwInitEGL() { return 0; }
void _glfwTerminateEGL() { }
int _glfwCreateContextEGL(void* a, const void* b, const void* c) { return 0; }
int _glfwChooseVisualEGL(const void* a, const void* b, const void* c, void** d, int* e) { return 0; }
")