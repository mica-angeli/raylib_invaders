#include <raylib.h>
#include "game.h"

int main() {
  Game g = {
      .screenWidth=800,
      .screenHeight=450
  };

  InitWindow(g.screenWidth, g.screenHeight, "Raylib Invaders");

  InitGame(&g);

  SetTargetFPS(60);

  while(!WindowShouldClose())
  {
    UpdateGame(&g);
    BeginDrawing();
    DrawGame(&g);
    EndDrawing();
  }

  CloseWindow();
  CloseGame(&g);
  return 0;
}
