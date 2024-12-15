#include "client/draw/window.h"
#include "client/input/input.h"
#include "client/menu/element/element.h"
#include "client/menu/menu.h"
#include "GLFW/glfw3.h"
#include "client/menu/type/pause_menu.h"

Input::Input(Game& game) : game(game) { }

bool pressed(int action) {
  return action == GLFW_PRESS || action == GLFW_REPEAT;
}

void Input::onPress(int key, int scancode, int action, int mods) {
  if (scancode == GLFW_MOUSE_PASSTHROUGH) {
    if (key == GLFW_MOUSE_BUTTON_1) {
      if (action == GLFW_PRESS) {
        Menu* menu = game.renderer->menu;
        if (menu != nullptr) {
          vec2i& mouse = game.window->mouse;
          menu->click(mouse.x(), mouse.y());
          return;
        }
      }

      shoot = pressed(action);
    }

    if (key == GLFW_MOUSE_BUTTON_2) spreadShoot = pressed(action);
    return;
  }

  if (key == GLFW_KEY_W) forward = pressed(action);
  if (key == GLFW_KEY_A) left = pressed(action);
  if (key == GLFW_KEY_S) backward = pressed(action);
  if (key == GLFW_KEY_D) right = pressed(action);
  if (key == GLFW_KEY_ESCAPE && pressed(action)) {
    Menu* menu = game.renderer->menu;
    if (menu == nullptr) {
      game.renderer->openMenu(new PauseMenu(*game.renderer));
    } else if (menu->closeable) {
      game.renderer->closeMenu();
    }
  }
  if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) scale = pressed(action) ? 20 : 10;

  Window* window = game.window;
  GLFWwindow* windowGLFW = window->window;
  if (action == GLFW_PRESS && key == GLFW_KEY_F11) {
    if (glfwGetWindowMonitor(windowGLFW) != nullptr) {
      glfwSetWindowMonitor(windowGLFW, nullptr, 32, 32, window->windowedResolution[0], window->windowedResolution[1], 0);
      window->rescale(window->windowedResolution[0], window->windowedResolution[1]);
    } else {
      GLFWmonitor* monitor = glfwGetPrimaryMonitor();
      const GLFWvidmode* mode = glfwGetVideoMode(monitor);
      glfwGetWindowSize(windowGLFW, &window->windowedResolution[0], &window->windowedResolution[1]);
      glfwSetWindowMonitor(windowGLFW, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
      window->rescale(mode->width, mode->height);
    }
  }
}

