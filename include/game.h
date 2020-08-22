#ifndef RAYLIB_INVADERS_GAME_H
#define RAYLIB_INVADERS_GAME_H

#include <raylib.h>

#define MAX_BULLETS 16
#define MAX_ENEMIES 50

typedef struct Entity
{
  Rectangle rect;
  Vector2 speed;
  Vector2 max_speed;
  bool active;
  Color color;
} Entity;

typedef struct Game
{
  const Rectangle screen;
  double lastShot;
  float shootRate;
  int activeEnemies;
  Entity player;
  Entity bullets[MAX_BULLETS];
  Entity enemies[MAX_ENEMIES];
  Sound sfxShoot;
  Sound sfxEnemyExplode;
  Sound sfxPlayerExplode;
} Game;

void InitGame(Game* g, double now);
void UpdateGame(Game* g, double now, float dt);
void DrawGame(Game* g);
void CloseGame(Game* g);

#endif //RAYLIB_INVADERS_GAME_H
