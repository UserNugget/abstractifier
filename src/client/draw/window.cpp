#include "stb_image.h"
#include "client/draw/window.h"
#include "client/input/input.h"
#include "client/texture/texture_manager.h"
#include "game.h"
#include "util/lib.h"

Window::Window(Game& game) : game(game) {
  LOG("initializing window...")

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

  resolution[0] = expectedResolution[0];
  resolution[1] = expectedResolution[1];
  window = glfwCreateWindow(resolution[0], resolution[1], NAME, nullptr, nullptr);
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
    it->resolution[0] = width;
    it->resolution[1] = height;
  });
  glfwSetCursorPosCallback(window, [](GLFWwindow* window, double posX, double posY) {
    Window* it = (Window*) glfwGetWindowUserPointer(window);
    it->mouse[0] = (int) posX;
    it->mouse[1] = it->resolution[1] - (int) posY;
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
}

Window::~Window() {
  LOG("closing window...")

  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::rescale(int width, int height) {
  resolution[0] = width;
  resolution[1] = height;

  double x = 0, y = 0;
  glfwGetCursorPos(window, &x, &y);

  mouse[0] = (int) x;
  mouse[1] = resolution[1] - (int) y;

  LOG("rescaled to " << mouse[0] << " " << mouse[1])
}
