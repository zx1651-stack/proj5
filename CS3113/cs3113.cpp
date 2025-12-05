/**
* Author: Zirui Xu
* Assignment: Swallow
* Date due: 2025-12-05, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "cs3113.h"

Color ColorFromHex(const char *hex)
{
    if (hex[0] == '#')
        hex++;

    unsigned int r = 0, g = 0, b = 0, a = 255;

    if (sscanf(hex, "%02x%02x%02x", &r, &g, &b) == 3)
    {
        return (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a};
    }
    else if (sscanf(hex, "%02x%02x%02x%02x", &r, &g, &b, &a) == 4)
    {
        return (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a};
    }
    else
    {
        return RAYWHITE;
    }
}

void Normalise(Vector2 *vector)
{
    float magnitude = GetLength(vector);

    vector->x /= magnitude;
    vector->y /= magnitude;
}

float GetLength(const Vector2 *vector)
{
    return sqrtf(pow(vector->x, 2) + pow(vector->y, 2));
}
