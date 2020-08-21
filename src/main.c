#include <raylib.h>
#include "game.h"

int main() {
  const int screen_width = 800;
  const int screen_height = 450;

  InitWindow(screen_width, screen_height, "Raylib Invaders");

  InitGame();

  SetTargetFPS(60);

  while(!WindowShouldClose())
  {
    UpdateGame();
    DrawGame();
  }

  CloseWindow();
  return 0;
}
