#include <stdio.h>
#include <stdlib.h>
#include "game.h"

static void SpawnBullet(Game *g);

void InitGame(Game* g)
{
  // Initialize player
  g->player.rect.x = g->screenWidth / 2.0f;
  g->player.rect.y = g->screenHeight - 20;
  g->player.rect.width = 20;
  g->player.rect.height = 20;
  g->player.speed.x = 5;
  g->player.speed.y = 5;
  g->player.color = BLACK;

  g->shootRate = 0;

  vector_ctor(&g->bullets);

  printf("Bullet size %d bytes", sizeof(Bullet));
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
  if(g->player.rect.x + g->player.rect.width >= g->screenWidth)
  {
    g->player.rect.x = g->screenWidth - g->player.rect.width;
  }

  if(IsKeyDown(KEY_SPACE))
  {
    g->shootRate += 5;
    SpawnBullet(g);
  }

  // Update Bullet positions
  for(int i = 0; i < vector_size(&g->bullets); i++)
  {
    Bullet* bullet = vector_at(&g->bullets, i);
    bullet->rect.x += bullet->speed.x;
    bullet->rect.y += bullet->speed.y;
  }


}

void DrawGame(Game* g)
{
  ClearBackground(RAYWHITE);

  DrawRectangleRec(g->player.rect, g->player.color);

  for(int i = 0; i < vector_size(&g->bullets); i++)
  {
    Bullet* bullet = vector_at(&g->bullets, i);
    if(bullet->active)
    {
      DrawRectangleRec(bullet->rect, bullet->color);
    }
  }
}

void CloseGame(Game* g)
{
  vector_dtor(&g->bullets);
}

static void SpawnBullet(Game *g)
{
  Bullet* bullet = malloc(sizeof(Bullet));
  bullet->rect.width = 5;
  bullet->rect.height = 10;
  bullet->rect.x = g->player.rect.x + g->player.rect.width / 2;
  bullet->rect.y = g->player.rect.y + g->player.rect.height / 4;
  bullet->speed.x = 0;
  bullet->speed.y = -10;
  bullet->active = true;
  bullet->color = MAROON;
  vector_push_back(&g->bullets, bullet);
}
