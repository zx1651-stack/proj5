/**
* Author: Zirui Xu
* Assignment: Swallow
* Date due: 2025-12-05, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "DrawL2Extras.h"
#include "L2Data.h"
#include "raylib.h"
#include <cmath>

static constexpr int   L2_LEVEL_WIDTH  = 70;
static constexpr int   L2_LEVEL_HEIGHT = 70;
static constexpr float L2_TILE_SIZE    = 75.0f;

static constexpr float LIGHTBALL_SCALE = 0.02f;
static constexpr float LIGHTBALL_SPEED = 400.0f;
static constexpr int   LIGHTBALL_COUNT = 12;

static Texture2D gLightBallTexture{};
static bool      gLightBallLoaded = false;

static Vector2 gLightBallSize{0.0f, 0.0f};

static Vector2 gLightBallPos[LIGHTBALL_COUNT];
static Vector2 gLightBallVel[LIGHTBALL_COUNT];

static bool gLightBallAlive[LIGHTBALL_COUNT];

static float gLeft   = 0.0f;
static float gRight  = 0.0f;
static float gTop    = 0.0f;
static float gBottom = 0.0f;

static bool gL2ExtrasInited = false;

static void ComputeLevelBounds(Vector2 levelOrigin)
{
    float halfInnerW = (L2_LEVEL_WIDTH - 2) * L2_TILE_SIZE * 0.5f;
    float halfInnerH = (L2_LEVEL_HEIGHT - 2) * L2_TILE_SIZE * 0.5f;

    gLeft   = levelOrigin.x - halfInnerW;
    gRight  = levelOrigin.x + halfInnerW;
    gTop    = levelOrigin.y - halfInnerH;
    gBottom = levelOrigin.y + halfInnerH;
}

void InitL2Extras(Vector2 levelOrigin)
{
    ComputeLevelBounds(levelOrigin);

    if (!gLightBallLoaded)
    {
        gLightBallTexture = LoadTexture("assets/lightball.png");
        gLightBallLoaded  = true;

        gLightBallSize = {gLightBallTexture.width * LIGHTBALL_SCALE, gLightBallTexture.height * LIGHTBALL_SCALE};
    }

    for (int i = 0; i < LIGHTBALL_COUNT; ++i)
    {
        gLightBallPos[i].x = LIGHTBALL_POS_X[i];
        gLightBallPos[i].y = LIGHTBALL_POS_Y[i];

        float angleRad     = LIGHTBALL_ANGLE[i] * (PI / 180.0f);
        gLightBallVel[i].x = cosf(angleRad) * LIGHTBALL_SPEED;
        gLightBallVel[i].y = sinf(angleRad) * LIGHTBALL_SPEED;

        gLightBallAlive[i] = true;
    }

    gL2ExtrasInited = true;
}

void UpdateLightBalls(float dt)
{
    if (!gL2ExtrasInited)
        return;
    if (!gLightBallLoaded)
        return;

    float halfW = gLightBallSize.x * 0.5f;
    float halfH = gLightBallSize.y * 0.5f;

    for (int i = 0; i < LIGHTBALL_COUNT; ++i)
    {
        if (!gLightBallAlive[i])
            continue;

        gLightBallPos[i].x += gLightBallVel[i].x * dt;
        gLightBallPos[i].y += gLightBallVel[i].y * dt;

        float x = gLightBallPos[i].x;
        float y = gLightBallPos[i].y;

        if (x - halfW <= gLeft)
        {
            x                  = gLeft + halfW;
            gLightBallVel[i].x = -gLightBallVel[i].x;
        }
        else if (x + halfW >= gRight)
        {
            x                  = gRight - halfW;
            gLightBallVel[i].x = -gLightBallVel[i].x;
        }

        if (y - halfH <= gTop)
        {
            y                  = gTop + halfH;
            gLightBallVel[i].y = -gLightBallVel[i].y;
        }
        else if (y + halfH >= gBottom)
        {
            y                  = gBottom - halfH;
            gLightBallVel[i].y = -gLightBallVel[i].y;
        }

        gLightBallPos[i].x = x;
        gLightBallPos[i].y = y;
    }
}

void DrawLightBalls()
{
    if (!gL2ExtrasInited)
        return;
    if (!gLightBallLoaded)
        return;

    Rectangle src = {0.0f, 0.0f, (float)gLightBallTexture.width, (float)gLightBallTexture.height};

    Vector2 origin = {gLightBallSize.x * 0.5f, gLightBallSize.y * 0.5f};

    for (int i = 0; i < LIGHTBALL_COUNT; ++i)
    {
        if (!gLightBallAlive[i])
            continue;

        Vector2 pos = gLightBallPos[i];

        Rectangle dst = {pos.x, pos.y, gLightBallSize.x, gLightBallSize.y};

        DrawTexturePro(gLightBallTexture, src, dst, origin, 0.0f, WHITE);
    }
}

void ShutdownL2Extras()
{
    if (gLightBallLoaded)
    {
        UnloadTexture(gLightBallTexture);
        gLightBallLoaded = false;
    }

    gL2ExtrasInited = false;
}

int GetTotalLightBallCount()
{
    return LIGHTBALL_COUNT;
}

bool IsLightBallAlive(int index)
{
    if (index < 0 || index >= LIGHTBALL_COUNT)
        return false;
    return gLightBallAlive[index];
}

Rectangle GetLightBallCollider(int index)
{
    Rectangle rect{0.0f, 0.0f, 0.0f, 0.0f};

    if (index < 0 || index >= LIGHTBALL_COUNT)
        return rect;
    if (!gLightBallAlive[index])
        return rect;

    Vector2 center = gLightBallPos[index];

    rect.width  = gLightBallSize.x;
    rect.height = gLightBallSize.y;
    rect.x      = center.x - gLightBallSize.x * 0.5f;
    rect.y      = center.y - gLightBallSize.y * 0.5f;

    return rect;
}

void EatLightBall(int index)
{
    if (index < 0 || index >= LIGHTBALL_COUNT)
        return;
    gLightBallAlive[index] = false;
}

void UpdateLightBallInteractions(const Rectangle &playerRect, int &score, Sound sfxGet)
{
    if (!gL2ExtrasInited)
        return;
    if (!gLightBallLoaded)
        return;

    for (int i = 0; i < LIGHTBALL_COUNT; ++i)
    {
        if (!gLightBallAlive[i])
            continue;

        Rectangle lr = GetLightBallCollider(i);
        if (lr.width <= 0.0f || lr.height <= 0.0f)
            continue;

        bool overlapX = (playerRect.x + playerRect.width > lr.x) && (playerRect.x < lr.x + lr.width);
        bool overlapY = (playerRect.y + playerRect.height > lr.y) && (playerRect.y < lr.y + lr.height);

        if (!overlapX || !overlapY)
            continue;

        EatLightBall(i);
        score += 15;
        PlaySound(sfxGet);
    }
}
