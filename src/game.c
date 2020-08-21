#include <stdio.h>
#include "game.h"

void InitGame()
{
  printf("Hello!");
}

void UpdateGame()
{

}

void DrawGame()
{
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawText("Raylib Invaders", 190, 200, 20, LIGHTGRAY);
  EndDrawing();
}
