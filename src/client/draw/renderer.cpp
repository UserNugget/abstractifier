#include "game.h"
#include "client/draw/shader.h"
#include "client/draw/renderer.h"
#include "client/draw/window.h"
#include "world/bullet.h"

Renderer::Renderer(Game &game) : game(game) {

}

void Renderer::draw() {
  float delta = (float) (timeMillis() - game.world->tickStart) / (1000.0f / (float) TICK_RATE);
  delta = std::max(std::min(delta, 1.0f), 0.0f); // clamp(delta, 0, 1);

  time = (float) glfwGetTime();

  glClearColor(0.5f, 0.5f, 0.5f, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Entity* client = game.world->at(0);
  if (client != nullptr) {
    vec2i& mouse = game.window->mouse;

    float resolutionX = ((float) game.window->expectedResolution[0]) / 2.0f;
    float resolutionY = ((float) game.window->expectedResolution[1]) / 2.0f;
    float mouseX = ((2.0f * (((float) mouse[0]) - game.window->viewport[0])) / game.window->viewport[2] - 1.0f) * resolutionX;
    float mouseY = ((2.0f * (((float) mouse[1]) - game.window->viewport[1])) / game.window->viewport[3] - 1.0f) * resolutionY;

    game.window->mouseWorld[0] = client->deltaX(delta) + mouseX;
    game.window->mouseWorld[1] = client->deltaY(delta) + mouseY;

    float scaleX = (float) game.window->expectedResolution[0];
    float scaleY = (float) game.window->expectedResolution[1];

    cameraPosition[0] = client->deltaX(delta) - (scaleX / 2.0f) + (client->w / 2.0f);
    cameraPosition[1] = client->deltaY(delta) - (scaleY / 2.0f) + (client->h / 2.0f);

    static Shader* background = new Shader("shaders/scale.vert", "shaders/background.frag");
    background->show();
    background->offset(cameraPosition);
    drawBuffer.pushSquare(0, 0, (float) game.window->expectedResolution[0], (float) game.window->expectedResolution[1]);
    drawBuffer.draw(game, *background);
    background->hide();

    int entities = 0;
    for (Entity* entity: game.world->entitiesSnapshot()) {
      if (!entity->outOfBounds(*this, delta)) {
        entity->renderTick(*this, delta);
        entities++;
      }
    }

    Entity::draw(*this);
    ClientEntity::draw(*this);
    Bullet::draw(*this);

    static Shader* hud = new Shader("shaders/scale.vert", "shaders/hud.frag");
    hud->show();
    hud->frameRate(frameRate);
    hud->tickTime((float) game.world->tickTime);
    hud->score((float) game.world->score);
    hud->entityCount((float) /*game.world->entities->size()*/ entities);
    drawBuffer.pushSquare(0, (float) game.window->expectedResolution[1] * 0.8f, (float) game.window->expectedResolution[0] * 0.4f, (float) game.window->expectedResolution[0] * 0.2f);
    drawBuffer.draw(game, *hud);
    hud->hide();
  }
}
