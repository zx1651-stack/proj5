#ifndef DRAWCANDIES_H
#define DRAWCANDIES_H

#include "raylib.h"

void InitCandies(Vector2 levelOrigin);

void DrawCandies();

void DrawViruses();

void ShutdownCandies();

int       GetTotalCandyCount();
bool      IsCandyAlive(int index);
Rectangle GetCandyCollider(int index);
void      EatCandy(int index);

int       GetTotalVirusCount();
bool      IsVirusAlive(int index);
Rectangle GetVirusCollider(int index);
void      RemoveVirus(int index);

void UpdateCandiesAndViruses(const Rectangle &playerRect, int &score, bool &outHitVirus, Sound sfxGet, Sound sfxHit);

void UpdateStaticVirusesForLevel3(float dt);

#endif
