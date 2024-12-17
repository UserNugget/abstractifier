#include "stb_image.h"
#include "client/draw/window.h"
#include "client/input/input.h"
#include "client/texture/texture_manager.h"
#include "client/menu/menu.h"
#include "game.h"
#include "util/lib.h"

Window::Window(Game& game) : game(game) {
  glfwSetErrorCallback([](int error, const char* description) {
    LOG("glfw error: " << description)
  });

  if (!glfwInit()) {
     LOG("failed to initialize glfw");
     glfwTerminate();
     exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_CONTEXT_NO_ERROR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  resolution = expectedResolution;
  window = glfwCreateWindow(resolution.x(), resolution.y(), NAME, nullptr, nullptr);
  if (!window) {
    LOG("failed to create window")
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND) {
    GLFWimage* logo = game.textureManager->load("textures/logo.png");
    if (logo != nullptr) {
      glfwSetWindowIcon(window, 1, logo);
      game.textureManager->free(logo);
    }
  }

  glfwSetWindowUserPointer(window, this);
  glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    Window* it = (Window*) glfwGetWindowUserPointer(window);
    it->resolution = { width, height };
  });
  glfwSetCursorPosCallback(window, [](GLFWwindow* window, double posX, double posY) {
    Window* it = (Window*) glfwGetWindowUserPointer(window);
    it->mouse = { (int) posX, it->resolution.y() - (int) posY };

    Menu* menu = it->game.renderer->menu;
    if (menu != nullptr) {
      menu->hover((float) it->mouse.x(), (float) it->mouse.y());
    }
  });
  glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* it = (Window*) glfwGetWindowUserPointer(window);
    it->game.input->onPress(key, scancode, action, mods);
  });
  glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
    Window* it = (Window*) glfwGetWindowUserPointer(window);
    it->game.input->onPress(button, GLFW_MOUSE_PASSTHROUGH, action, mods);
  });

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  gladLoadGL(glfwGetProcAddress);

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  if (monitor) {
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (mode) {
      monitorFrameRate = fmax(mode->refreshRate, 60);
    }
  }
}

Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::rescale(int width, int height) {
  double x = 0, y = 0;
  glfwGetCursorPos(window, &x, &y);

  resolution = { width, height };
  mouse = { (int) x, resolution.y() - (int) y };
}
