
#ifndef DRAWL2EXTRAS_H
#define DRAWL2EXTRAS_H

#include "raylib.h"

void InitL2Extras(Vector2 levelOrigin);
void UpdateLightBalls(float dt);
void DrawLightBalls();
void ShutdownL2Extras();

int       GetTotalLightBallCount();
bool      IsLightBallAlive(int index);
Rectangle GetLightBallCollider(int index);
void      EatLightBall(int index);

void UpdateLightBallInteractions(const Rectangle &playerRect, int &score,
                                 Sound sfxGet); // [L2]

#endif
