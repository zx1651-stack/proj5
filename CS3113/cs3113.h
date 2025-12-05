/**
* Author: Zirui Xu
* Assignment: Lunar Lander
* Date due: 2025-10-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef CS3113_H
#define CS3113_H

#include "raylib.h"
#include <map>
#include <math.h>
#include <stdio.h>
#include <string>
#include <time.h>
#include <vector>

enum AppStatus
{
    TERMINATED,
    RUNNING
};

Color ColorFromHex(const char *hex);
void  Normalise(Vector2 *vector);
float GetLength(const Vector2 *vector);

#endif