#ifndef RAYLIB_INVADERS_GAME_H
#define RAYLIB_INVADERS_GAME_H

#include <raylib.h>

typedef struct Player
{
  Rectangle rect;
  Vector2 speed;
  Color color;
} Player;

typedef struct Game
{
  const int screenWidth;
  const int screenHeight;
  Player player;
} Game;

void InitGame(Game* g);
void UpdateGame(Game* g);
void DrawGame(Game* g);

#endif //RAYLIB_INVADERS_GAME_H
