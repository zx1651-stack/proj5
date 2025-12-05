/**
* Author: Zirui Xu
* Assignment: Swallow
* Date due: 2025-12-05, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

// CS3113/DrawCandies.cpp
#include "DrawCandies.h"
#include "CandyData.h"
#include "raylib.h"

static constexpr float CANDY_SCALE_FACTOR   = 0.03f;
static constexpr float CANDY_COLLIDER_SCALE = 0.8f;

static constexpr int TOTAL_CANDY_COUNT = RANDOM_CANDY_COUNT;

static constexpr float VIRUS_SCALE_FACTOR   = 0.4f;
static constexpr float VIRUS_COLLIDER_SCALE = 0.7f;

static constexpr int VIRUS_ATLAS_ROWS = 1;
static constexpr int VIRUS_ATLAS_COLS = 4;
static constexpr int VIRUS_ANIM_FPS   = 12;

static constexpr int TOTAL_VIRUS_COUNT = 20;

static constexpr float VIRUS_PATH_SPEED = 60.0f;

static Texture2D gCandyTexture{};
static bool      gCandyLoaded = false;

static Vector2 gCandySize{0.0f, 0.0f};
static bool    gCandyAlive[TOTAL_CANDY_COUNT];

static Texture2D gVirusTexture{};
static bool      gVirusLoaded = false;

static Vector2 gVirusSize{0.0f, 0.0f};

static Vector2 gVirusCenters[TOTAL_VIRUS_COUNT];

static bool gVirusAlive[TOTAL_VIRUS_COUNT];

static int   gVirusPathStep[TOTAL_VIRUS_COUNT];
static float gVirusPathProgress[TOTAL_VIRUS_COUNT];

static inline bool rectOverlap(const Rectangle &a, const Rectangle &b)
{
    bool overlapX = (a.x + a.width > b.x) && (a.x < b.x + b.width);
    bool overlapY = (a.y + a.height > b.y) && (a.y < b.y + b.height);
    return overlapX && overlapY;
}

static Vector2 GetCandyCenter(int index)
{
    if (index < 0 || index >= TOTAL_CANDY_COUNT)
    {
        return {0.0f, 0.0f};
    }
    return {CANDY_POS_X[index], CANDY_POS_Y[index]};
}

static float GetCandyAngleInternal(int index)
{
    if (index < 0 || index >= TOTAL_CANDY_COUNT)
    {
        return 0.0f;
    }
    return CANDY_ANGLE[index];
}

static Vector2 GetVirusCenter(int index)
{
    if (index < 0 || index >= TOTAL_VIRUS_COUNT)
    {
        return {0.0f, 0.0f};
    }
    return gVirusCenters[index];
}

void InitCandies(Vector2 levelOrigin)
{
    (void)levelOrigin;

    if (!gCandyLoaded)
    {
        gCandyTexture = LoadTexture("assets/candy.png");
        gCandyLoaded  = true;

        gCandySize = {gCandyTexture.width * CANDY_SCALE_FACTOR, gCandyTexture.height * CANDY_SCALE_FACTOR};
    }

    for (int i = 0; i < TOTAL_CANDY_COUNT; ++i)
    {
        gCandyAlive[i] = true;
    }

    if (!gVirusLoaded)
    {
        gVirusTexture = LoadTexture("assets/virus_spritesheet.png");
        gVirusLoaded  = true;

        float frameWidth  = (float)gVirusTexture.width / (float)VIRUS_ATLAS_COLS;
        float frameHeight = (float)gVirusTexture.height / (float)VIRUS_ATLAS_ROWS;

        gVirusSize = {frameWidth * VIRUS_SCALE_FACTOR, frameHeight * VIRUS_SCALE_FACTOR};
    }

    for (int i = 0; i < TOTAL_VIRUS_COUNT; ++i)
    {
        gVirusCenters[i] = {VIRUS_POS_X[i], VIRUS_POS_Y[i]};
        gVirusAlive[i]   = true;

        gVirusPathStep[i]     = 0;
        gVirusPathProgress[i] = 0.0f;
    }
}

void DrawCandies()
{
    if (!gCandyLoaded)
        return;

    Rectangle src = {0.0f, 0.0f, (float)gCandyTexture.width, (float)gCandyTexture.height};

    Vector2 origin = {gCandySize.x * 0.5f, gCandySize.y * 0.5f};

    for (int i = 0; i < TOTAL_CANDY_COUNT; ++i)
    {
        if (!gCandyAlive[i])
            continue;

        Vector2 center = GetCandyCenter(i);
        float   angle  = GetCandyAngleInternal(i);

        Rectangle dst = {center.x, center.y, gCandySize.x, gCandySize.y};

        DrawTexturePro(gCandyTexture, src, dst, origin, angle, WHITE);
    }
}

void DrawViruses()
{
    if (!gVirusLoaded)
        return;

    float t      = (float)GetTime();
    float frameF = t * (float)VIRUS_ANIM_FPS;
    int   frame  = (int)frameF % VIRUS_ATLAS_COLS;

    float frameWidth  = (float)gVirusTexture.width / (float)VIRUS_ATLAS_COLS;
    float frameHeight = (float)gVirusTexture.height / (float)VIRUS_ATLAS_ROWS;

    Rectangle src = {frame * frameWidth, 0.0f, frameWidth, frameHeight};

    Vector2 origin = {gVirusSize.x * 0.5f, gVirusSize.y * 0.5f};

    for (int i = 0; i < TOTAL_VIRUS_COUNT; ++i)
    {
        if (!gVirusAlive[i])
            continue;

        Vector2 center = GetVirusCenter(i);

        Rectangle dst = {center.x, center.y, gVirusSize.x, gVirusSize.y};

        DrawTexturePro(gVirusTexture, src, dst, origin, 0.0f, WHITE);
    }
}

void ShutdownCandies()
{
    if (gCandyLoaded)
    {
        UnloadTexture(gCandyTexture);
        gCandyLoaded = false;
    }

    if (gVirusLoaded)
    {
        UnloadTexture(gVirusTexture);
        gVirusLoaded = false;
    }
}

int GetTotalCandyCount()
{
    return TOTAL_CANDY_COUNT;
}

bool IsCandyAlive(int index)
{
    if (index < 0 || index >= TOTAL_CANDY_COUNT)
        return false;
    return gCandyAlive[index];
}

Rectangle GetCandyCollider(int index)
{
    Rectangle rect{0, 0, 0, 0};

    if (index < 0 || index >= TOTAL_CANDY_COUNT)
    {
        return rect;
    }

    Vector2 center = GetCandyCenter(index);

    float w = gCandySize.x * CANDY_COLLIDER_SCALE;
    float h = gCandySize.y * CANDY_COLLIDER_SCALE;

    rect.width  = w;
    rect.height = h;
    rect.x      = center.x - w * 0.5f;
    rect.y      = center.y - h * 0.5f;

    return rect;
}

void EatCandy(int index)
{
    if (index < 0 || index >= TOTAL_CANDY_COUNT)
        return;
    gCandyAlive[index] = false;
}

int GetTotalVirusCount()
{
    return TOTAL_VIRUS_COUNT;
}

bool IsVirusAlive(int index)
{
    if (index < 0 || index >= TOTAL_VIRUS_COUNT)
        return false;
    return gVirusAlive[index];
}

Rectangle GetVirusCollider(int index)
{
    Rectangle rect{0, 0, 0, 0};

    if (index < 0 || index >= TOTAL_VIRUS_COUNT)
    {
        return rect;
    }

    Vector2 center = GetVirusCenter(index);

    float w = gVirusSize.x * VIRUS_COLLIDER_SCALE;
    float h = gVirusSize.y * VIRUS_COLLIDER_SCALE;

    rect.width  = w;
    rect.height = h;
    rect.x      = center.x - w * 0.5f;
    rect.y      = center.y - h * 0.5f;

    return rect;
}

void RemoveVirus(int index)
{
    if (index < 0 || index >= TOTAL_VIRUS_COUNT)
        return;
    gVirusAlive[index] = false;
}

void UpdateStaticVirusesForLevel3(float dt)
{
    if (!gVirusLoaded)
        return;

    float baseDistance = VIRUS_PATH_SPEED * dt;

    for (int i = 0; i < TOTAL_VIRUS_COUNT; ++i)
    {
        if (!gVirusAlive[i])
            continue;

        float remaining = baseDistance;

        while (remaining > 0.0f)
        {
            int   step = gVirusPathStep[i];
            float legLength;
            int   dir;

            if (step == 0)
            {
                legLength = 50.0f;
                dir       = -1;
            }
            else if (step == 1)
            {
                legLength = 100.0f;
                dir       = 1;
            }
            else if (step == 2)
            {
                legLength = 100.0f;
                dir       = -1;
            }
            else
            {
                legLength = 100.0f;
                dir       = 1;
            }

            float leftInLeg = legLength - gVirusPathProgress[i];

            if (remaining < leftInLeg)
            {
                float move = remaining * (float)dir;
                gVirusCenters[i].x += move;
                gVirusPathProgress[i] += remaining;
                remaining = 0.0f;
            }
            else
            {
                float move = leftInLeg * (float)dir;
                gVirusCenters[i].x += move;
                gVirusPathProgress[i] = 0.0f;

                step += 1;
                if (step >= 4)
                    step = 0;
                gVirusPathStep[i] = step;

                remaining -= leftInLeg;
            }
        }
    }
}

void UpdateCandiesAndViruses(const Rectangle &playerRect, int &score, bool &outHitVirus, Sound sfxGet, Sound sfxHit)
{
    (void)sfxGet;
    (void)sfxHit;

    for (int i = 0; i < TOTAL_CANDY_COUNT; ++i)
    {
        if (!gCandyAlive[i])
            continue;

        Rectangle cRect = GetCandyCollider(i);
        if (!rectOverlap(playerRect, cRect))
            continue;

        gCandyAlive[i] = false;
        score += 1;
    }

    bool hitVirus = false;
    for (int i = 0; i < TOTAL_VIRUS_COUNT; ++i)
    {
        if (!gVirusAlive[i])
            continue;

        Rectangle vRect = GetVirusCollider(i);
        if (!rectOverlap(playerRect, vRect))
            continue;

        hitVirus = true;
        break;
    }

    outHitVirus = hitVirus;
}
