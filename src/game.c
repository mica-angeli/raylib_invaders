#include <stdio.h>
#include "game.h"

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
}

void DrawGame(Game* g)
{
  ClearBackground(RAYWHITE);

  DrawRectangleRec(g->player.rect, g->player.color);
}
