#ifndef RAYLIB_INVADERS_GAME_H
#define RAYLIB_INVADERS_GAME_H

#include <raylib.h>
#include "vector.h"

#define MAX_BULLETS 16

typedef struct Player
{
  Rectangle rect;
  Vector2 speed;
  Color color;
} Player;

typedef struct Bullet
{
  Rectangle rect;
  Vector2 speed;
  bool active;
  Color color;
} Bullet;

typedef struct Game
{
  const Rectangle screen;
  int shootRate;
  Player player;
  Bullet bullets[MAX_BULLETS];
} Game;

void InitGame(Game* g);
void UpdateGame(Game* g);
void DrawGame(Game* g);
void CloseGame(Game* g);

#endif //RAYLIB_INVADERS_GAME_H
