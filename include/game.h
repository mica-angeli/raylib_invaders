#ifndef RAYLIB_INVADERS_GAME_H
#define RAYLIB_INVADERS_GAME_H

#include <raylib.h>

#define MAX_BULLETS 16

typedef struct Entity
{
  Rectangle rect;
  Vector2 speed;
  bool active;
  Color color;
} Entity;

typedef struct Game
{
  const Rectangle screen;
  int shootRate;
  Entity player;
  Entity bullets[MAX_BULLETS];
} Game;

void InitGame(Game* g);
void UpdateGame(Game* g);
void DrawGame(Game* g);
void CloseGame(Game* g);

#endif //RAYLIB_INVADERS_GAME_H
