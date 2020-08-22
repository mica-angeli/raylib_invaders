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

void InitGame(Game* g)
{
  // Initialize player
  Entity* player = &g->player;
  player->rect.x = g->screen.width / 2.0f;
  player->rect.y = g->screen.height - 20;
  player->rect.width = 20;
  player->rect.height = 20;
  player->max_speed.x = 300;
  player->max_speed.y = 0;
  player->speed.x = 0;
  player->speed.y = 0;
  player->active = true;
  player->color = BLACK;

  g->timeUntilNextShot = 0;
  g->shootRate = 3;

  // Initialize bullets
  foreach(Entity* bullet, g->bullets)
  {
    bullet->rect.x = 0;
    bullet->rect.y = 0;
    bullet->rect.width = 5;
    bullet->rect.height = 10;
    bullet->max_speed.x = 0;
    bullet->max_speed.y = 600;
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
    enemy->max_speed.y = 200.0f + 6.0f * (float) GetRandomValue(-10, 10);
    enemy->speed.x = 0;
    enemy->speed.y = 0;
    enemy->active = true;
    enemy->color = BLUE;
  }

  g->sfxShoot = LoadSound("../Resources/shoot.wav");
  g->sfxEnemyExplode = LoadSound("../Resources/enemy_explosion.wav");
  g->sfxPlayerExplode = LoadSound("../Resources/player_explosion.wav");

  TraceLog(LOG_INFO, "Game object size %lu bytes\n", sizeof(Game));
}

void UpdateGame(Game* g, float dt)
{
  // Update player speed
  g->player.speed.x = g->player.max_speed.x *
                      (float) (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT)) *
                      g->player.active;
  g->player.speed.y = g->player.max_speed.y * g->player.active;


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

  // Fire bullets when space bar is pressed at a certain rate and player is still alive
  g->timeUntilNextShot -= dt;
  if(IsKeyDown(KEY_SPACE) &&
    g->timeUntilNextShot <= 0 &&
    g->player.active)
  {
    g-> timeUntilNextShot = 1.0f / g->shootRate;
    PlaySound(g->sfxShoot);

    foreach(Entity* bullet, g->bullets)
    {
      if(!bullet->active)
      {
        const Vector2 player_center = RectangleCenter(&g->player.rect);

        bullet->rect.x = player_center.x - bullet->rect.width / 2;
        bullet->rect.y = g->player.rect.y;
        bullet->speed.x = bullet->max_speed.x + g->player.speed.x / 4;
        bullet->speed.y = -bullet->max_speed.y;
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
            PlaySound(g->sfxEnemyExplode);

            // Reset enemy position
            enemy->rect.x = (float) GetRandomValue(g->screen.width, g->screen.width + 1000);
            enemy->rect.y = (float) GetRandomValue(0, g->screen.height - enemy->rect.height);
            g->timeUntilNextShot = 0;
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

      // Check if enemy goes off screen
      if(!CheckCollisionRecs(enemy->rect, g->screen))
      {
        enemy->rect.x = (float) GetRandomValue(0, (int) g->screen.width);
        enemy->rect.y = (float) GetRandomValue((int) -g->screen.height, -20);
      }

      // Check if enemy collides with player
      if(g->player.active && CheckCollisionRecs(enemy->rect, g->player.rect))
      {
        PlaySound(g->sfxPlayerExplode);

        enemy->rect.x = (float) GetRandomValue(0, (int) g->screen.width);
        enemy->rect.y = (float) GetRandomValue((int) -g->screen.height, -20);

        g->player.active = false;
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

  if(g->player.active)
  {
    DrawRectangleRec(g->player.rect, g->player.color);
  }

}

void CloseGame(Game* g)
{
  UnloadSound(g->sfxShoot);
  UnloadSound(g->sfxEnemyExplode);
  UnloadSound(g->sfxPlayerExplode);
}

static Vector2 RectangleCenter(const Rectangle* rect)
{
  const Vector2 center = {
      .x = rect->x + rect->width / 2.0f,
      .y = rect->y + rect->height / 2.0f
  };
  return center;
}
