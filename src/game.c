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
static bool CheckCollisionRectPoint(const Rectangle* r, const Vector2* p);
static bool CheckCollisionRectRect(const Rectangle* r1, const Rectangle* r2);

void InitGame(Game* g)
{
  // Initialize player
  g->player.rect.x = g->screen.width / 2.0f;
  g->player.rect.y = g->screen.height - 20;
  g->player.rect.width = 20;
  g->player.rect.height = 20;
  g->player.speed.x = 0;
  g->player.speed.y = 0;
  g->player.color = BLACK;

  g->shootRate = 0;

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

  printf("Game object size %lu bytes\n", sizeof(Game));
}

void UpdateGame(Game* g)
{
  // Update player speed
  g->player.speed.x = 5 * (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT));

  // Update player position
  g->player.rect.x += g->player.speed.x;
  g->player.rect.y += g->player.speed.y;

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
  if(IsKeyDown(KEY_SPACE))
  {
    g->shootRate += 5;

    foreach(Entity* bullet, g->bullets)
    {
      if(!bullet->active && g->shootRate % 40 == 0)
      {
        const Vector2 player_center = RectangleCenter(&g->player.rect);

        bullet->rect.x = player_center.x - bullet->rect.width / 2;
        bullet->rect.y = g->player.rect.y;
        bullet->speed.y = -10;
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
      bullet->rect.x += bullet->speed.x;
      bullet->rect.y += bullet->speed.y;

      // Check if bullet has left the screen
      if(!CheckCollisionRectRect(&bullet->rect, &g->screen))
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

  foreach(Entity* bullet, g->bullets)
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

static bool CheckCollisionRectPoint(const Rectangle* r, const Vector2* p)
{
  return p->x >= r->x && p->x <= r->x + r->width &&
    p->y >= r->y && p->y <= r->y + r->height;
}

static bool CheckCollisionRectRect(const Rectangle* r1, const Rectangle* r2)
{
  // Array of corners in r2
  const Vector2 r2_corners[] = {
    {r2->x, r2->y},
    {r2->x + r2->width, r2->y},
    {r2->x, r2->y + r2->height},
    {r2->x + r2->width, r2->y + r2->height}
  };

  // Check if any corner of r2 is within r1
  foreach(const Vector2* p, r2_corners)
  {
    if(CheckCollisionRectPoint(r1, p))
      return true;
  }

  // Array of corners in r1
  const Vector2 r1_corners[] = {
      {r1->x, r1->y},
      {r1->x + r1->width, r1->y},
      {r1->x, r1->y + r1->height},
      {r1->x + r1->width, r1->y + r1->height}
  };

  // Check if any corner of r1 is within r2
  foreach(const Vector2* p, r1_corners)
  {
    if(CheckCollisionRectPoint(r2, p))
      return true;
  }

  return false;
}
