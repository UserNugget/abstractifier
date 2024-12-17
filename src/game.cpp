#include "client/input/input.h"
#include "client/draw/window.h"
#include "client/draw/renderer.h"
#include "client/texture/texture_manager.h"
#include "world/world.h"
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
    int width = window->resolution.x(), height = window->resolution.y();

    int expectedWidth = window->expectedResolution.x();
    int expectedHeight = window->expectedResolution.y();

    float ratioX = (float) width / (float) expectedWidth;
    float ratioY = (float) height / (float) expectedHeight;
    float ratio = ratioX < ratioY ? ratioX : ratioY;
    
    int viewWidth = (int) ((float) expectedWidth * ratio);
    int viewHeight = (int) ((float) expectedHeight * ratio);
    
    int viewX = (int) (((float) width - (float) expectedWidth * ratio) / 2);
    int viewY = (int) (((float) height - (float) expectedHeight * ratio) / 2);

    window->viewport[0] = (float) viewX;
    window->viewport[1] = (float) viewY;
    window->viewport[2] = (float) viewWidth;
    window->viewport[3] = (float) viewHeight;
    window->viewport[4] = ratioX;
    window->viewport[5] = ratioY;

    glViewport(viewX, viewY, viewWidth, viewHeight);

    renderer->draw();

    glfwPollEvents();
    glfwSwapBuffers(window->window);

    static double previousTickDelta = glfwGetTime();

    double time = glfwGetTime();
    double timePassed = time - previousTickDelta;
    if (timePassed >= 1.0) {
      previousTickDelta = time;
      renderer->frameRate = (float) (frames * (1.0 / timePassed));
      frames = 0;
    }

    double waitFor = renderer->time + (1.0 / window->monitorFrameRate);
    while (time < waitFor) {
      glfwWaitEventsTimeout(waitFor - time);
      time = glfwGetTime();
    }

    frames++;
  }

  this->quit = true;
}

void Game::tick() {
  std::thread tickerThread(World::tick, world);
  tickerThread.detach();
}
