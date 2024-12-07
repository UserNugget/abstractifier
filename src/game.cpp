#include <iostream>
#include "client/input/input.h"
#include "client/draw/window.h"
#include "client/draw/renderer.h"
#include "client/texture/texture_manager.h"
#include "world/world.h"
#include "util/lib.h"
#include "game.h"

Game::Game() {
  window = new Window(*this);
  input = new Input(*this);
  renderer = new Renderer(*this);
  textureManager = new TextureManager(*this);
  world = new ClientWorld(*this);
}

Game::~Game() {
  delete window;
  delete input;
  delete renderer;
  delete textureManager;
  delete world;
}


void Game::render() {
  int frames = 0;
  while (!glfwWindowShouldClose(window->window)) {
    int width, height;
    glfwGetFramebufferSize(window->window, &width, &height);
    
    int expectedWidth = window->expectedResolution[0];
    int expectedHeight = window->expectedResolution[1];

    float ratioX = (float) width / (float) expectedWidth;
    float ratioY = (float) height / (float) expectedHeight;
    float ratio = ratioX < ratioY ? ratioX : ratioY;
    
    int viewWidth = (int) ((float) expectedWidth * ratio);
    int viewHeight = (int) ((float) expectedHeight * ratio);
    
    int viewX = (int) (((float) width - (float) expectedWidth * ratio) / 2);
    int viewY = (int) (((float) height - (float) expectedHeight * ratio) / 2);

    window->viewpoint[0] = (float) viewX;
    window->viewpoint[1] = (float) viewY;
    window->viewpoint[2] = (float) viewWidth;
    window->viewpoint[3] = (float) viewHeight;
    window->viewpoint[4] = ratioX;
    window->viewpoint[5] = ratioY;

    glViewport(viewX, viewY, viewWidth, viewHeight);

    renderer->draw(ratioX, ratioY);

    glfwSwapBuffers(window->window);
    glfwSwapInterval(0);
    glfwPollEvents();

    static double previousDelta = glfwGetTime();
    static double previousTickDelta = glfwGetTime();
    if (FRAME_RATE != 0) {
      double delta = previousDelta + (1.0 / FRAME_RATE);

      previousDelta = glfwGetTime();
      while (previousDelta < delta) {
        glfwWaitEventsTimeout(delta - previousDelta);
        previousDelta = glfwGetTime();
      }
    }

    if (glfwGetTime() - previousTickDelta >= 1.0) {
      previousTickDelta = glfwGetTime();
      renderer->frameRate = (float) frames;
      LOG("frame rate: " << frames)
      frames = 0;
    }

    frames++;
  }

  this->quit = true;
}

void Game::tick() {
  std::thread tickerThread(World::tick, world);
  tickerThread.detach();
}
