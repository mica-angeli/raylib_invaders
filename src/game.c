#include <stdio.h>
#include <stdlib.h>
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
  g->player.rect.x = g->screen.width / 2.0f;
  g->player.rect.y = g->screen.height - 20;
  g->player.rect.width = 20;
  g->player.rect.height = 20;
  g->player.speed.x = 5;
  g->player.speed.y = 5;
  g->player.color = BLACK;

  g->shootRate = 0;

  // Initialize bullets
  foreach(Bullet* bullet, g->bullets)
  {
    bullet->rect.x = 0;
    bullet->rect.y = 0;
    bullet->rect.width = 5;
    bullet->rect.height = 10;
    bullet->speed.x = 0;
    bullet->speed.y = -10;
    bullet->active = false;
    bullet->color = MAROON;
  }

  printf("Game object size %d bytes\n", sizeof(Game));
}

void UpdateGame(Game* g)
{
  // Player movement
  if(IsKeyDown(KEY_RIGHT))
  {
    g->player.rect.x += g->player.speed.x;
  }
  if(IsKeyDown(KEY_LEFT))
  {
    g->player.rect.x -= g->player.speed.x;
  }

  // Wall behavior
  if(g->player.rect.x <= 0)
  {
    g->player.rect.x = 0;
  }
  if(g->player.rect.x + g->player.rect.width >= g->screen.width)
  {
    g->player.rect.x = g->screen.width - g->player.rect.width;
  }

  if(IsKeyDown(KEY_SPACE))
  {
    g->shootRate += 5;

    foreach(Bullet* bullet, g->bullets)
    {
      if(!bullet->active && g->shootRate % 40 == 0)
      {
        const Vector2 player_center = RectangleCenter(&g->player.rect);
        const float buffer = 10;

        bullet->rect.x = player_center.x - bullet->rect.width / 2;
        bullet->rect.y = player_center.y - buffer - bullet->rect.height;
        bullet->active = true;
        break;
      }
    }
  }

  // Update Bullet positions
  foreach(Bullet* bullet, g->bullets)
  {
    if(bullet->active)
    {
      bullet->rect.x += bullet->speed.x;
      bullet->rect.y += bullet->speed.y;

      if(bullet->rect.y <= 0)
      {
        bullet->active = false;
        g->shootRate = 0;
      }
    }
  }
}

void DrawGame(Game* g)
{
  ClearBackground(RAYWHITE);

  DrawRectangleRec(g->player.rect, g->player.color);

  foreach(Bullet* bullet, g->bullets)
  {
    if(bullet->active)
    {
      DrawRectangleRec(bullet->rect, bullet->color);
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
