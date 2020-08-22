#include <stdio.h>
#include "game.h"

#define foreach(item, array) \
    for(int keep = 1, \
            count = 0,\
            size = sizeof (array) / sizeof *(array); \
        keep && count != size; \
        keep = !keep, count++) \
      for(item = (array) + count; keep; keep = !keep)

static Vector2 RectangleCenter(const Rectangle* rect);

void InitGame(Game* g, double now)
{
  // Initialize player
  g->player.rect.x = g->screen.width / 2.0f;
  g->player.rect.y = g->screen.height - 20;
  g->player.rect.width = 20;
  g->player.rect.height = 20;
  g->player.speed.x = 0;
  g->player.speed.y = 0;
  g->player.color = BLACK;

  g->lastShot = 0;
  g->shootRate = 3;

  // Initialize bullets
  foreach(Entity* bullet, g->bullets)
  {
    bullet->rect.x = 0;
    bullet->rect.y = 0;
    bullet->rect.width = 5;
    bullet->rect.height = 10;
    bullet->speed.x = 0;
    bullet->speed.y = 0;
    bullet->active = false;
    bullet->color = MAROON;
  }

  g->activeEnemies = 20;

  // Initialize enemies
  foreach(Entity* enemy, g->enemies)
  {
    enemy->rect.width = 20;
    enemy->rect.height = 20;
    enemy->rect.x = (float) GetRandomValue(0, (int) g->screen.width);
    enemy->rect.y = (float) GetRandomValue((int) -g->screen.height, -20);
    enemy->max_speed.x = 0;
    enemy->max_speed.y = 300.0f + 6.0f * (float) GetRandomValue(-10, 10);
    enemy->speed.x = 0;
    enemy->speed.y = 0;
    enemy->active = true;
    enemy->color = BLUE;
  }

  TraceLog(LOG_INFO, "Game object size %lu bytes\n", sizeof(Game));
}

void UpdateGame(Game* g, double now, float dt)
{
  // Update player speed
  g->player.speed.x = 300.0f * (float) (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT));

  // Update player position
  g->player.rect.x += g->player.speed.x * dt;
  g->player.rect.y += g->player.speed.y * dt;

  // Wall behavior
  if(g->player.rect.x <= 0)
  {
    g->player.rect.x = 0;
  }
  if(g->player.rect.x + g->player.rect.width >= g->screen.width)
  {
    g->player.rect.x = g->screen.width - g->player.rect.width;
  }

  // Fire bullets
  if(IsKeyDown(KEY_SPACE) && (now - g->lastShot) > 1.0f / g->shootRate)
  {
    g->lastShot = now;

    foreach(Entity* bullet, g->bullets)
    {
      if(!bullet->active)
      {
        const Vector2 player_center = RectangleCenter(&g->player.rect);

        bullet->rect.x = player_center.x - bullet->rect.width / 2;
        bullet->rect.y = g->player.rect.y;
        bullet->speed.x = g->player.speed.x / 4;
        bullet->speed.y = -600;
        bullet->active = true;
        break;
      }
    }
  }

  // Update bullet positions
  foreach(Entity* bullet, g->bullets)
  {
    if(bullet->active)
    {
      bullet->rect.x += bullet->speed.x * dt;
      bullet->rect.y += bullet->speed.y * dt;

      // Check if bullet has left the screen
      if(!CheckCollisionRecs(bullet->rect, g->screen))
      {
        bullet->active = false;
      }

      // Check if bullet has collided with an enemy
      for(int i = 0; i < g->activeEnemies; i++)
      {
        Entity *enemy = &g->enemies[i];
        if(enemy->active)
        {
          if(CheckCollisionRecs(bullet->rect, enemy->rect))
          {
            bullet->active = false;

            // Reset enemy position
            enemy->rect.x = (float) GetRandomValue(g->screen.width, g->screen.width + 1000);
            enemy->rect.y = (float) GetRandomValue(0, g->screen.height - enemy->rect.height);
            g->lastShot = 0;
          }
        }
      }
    }
  }

  for(int i = 0; i < g->activeEnemies; i++)
  {
    Entity* enemy = &g->enemies[i];
    if(enemy->active)
    {
      enemy->speed.x = enemy->max_speed.x;
      enemy->speed.y = enemy->max_speed.y;

      enemy->rect.x += enemy->speed.x * dt;
      enemy->rect.y += enemy->speed.y * dt;

      if(!CheckCollisionRecs(enemy->rect, g->screen))
      {
        enemy->rect.x = (float) GetRandomValue(0, (int) g->screen.width);
        enemy->rect.y = (float) GetRandomValue((int) -g->screen.height, -20);
      }
    }
    else
    {
      enemy->speed.x = 0;
      enemy->speed.y = 0;
    }
  }
}

void DrawGame(Game* g)
{
  ClearBackground(RAYWHITE);

  DrawRectangleRec(g->player.rect, g->player.color);

  foreach(Entity* bullet, g->bullets)
  {
    if(bullet->active)
    {
      DrawRectangleRec(bullet->rect, bullet->color);
    }
  }

  for(int i = 0; i < g->activeEnemies; i++)
  {
    Entity *enemy = &g->enemies[i];
    if(enemy->active)
    {
      DrawRectangleRec(enemy->rect, enemy->color);
    }
  }

}

void CloseGame(Game* g)
{

}

static Vector2 RectangleCenter(const Rectangle* rect)
{
  const Vector2 center = {
      .x = rect->x + rect->width / 2.0f,
      .y = rect->y + rect->height / 2.0f
  };
  return center;
}
