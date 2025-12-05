/**
* Author: Zirui Xu
* Assignment: Swallow
* Date due: 2025-12-05, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Level3Scene.h"
#include "raylib.h"
#include <cmath>
#include <vector>

#include "DrawCandies.h"
#include "DrawL2Extras.h"
#include "Entity.h"
#include "Map.h"

extern int gScore;

namespace
{
constexpr int   LEVEL_WIDTH  = 70;
constexpr int   LEVEL_HEIGHT = 70;
constexpr float TILE_SIZE    = 75.0f;

constexpr int BALL_SPEED = 300;

constexpr int BALL_ATLAS_ROWS = 1;
constexpr int BALL_ATLAS_COLS = 4;
constexpr int BALL_ANIM_FPS   = 8;

constexpr float BALL_SCALE_FACTOR = 0.4f;

constexpr int LEVEL3_SCORE_LINE = 450;

// L3
constexpr float VIRUS_MOVE_FIRST_SPAWN_TIME = 5.0f;
constexpr float VIRUS_MOVE_RESPAWN_INTERVAL = 10.0f;
constexpr float VIRUS_MOVE_OFFSET_DISTANCE  = 400.0f;
constexpr float VIRUS_MOVE_SPEED_FACTOR     = 1.15f;
constexpr float VIRUS_MOVE_SCALE            = 0.30f;
constexpr float VIRUS_MOVE_COLLIDER_SCALE   = 1.0f;

float fclamp(float v, float lo, float hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

void buildLevelData(unsigned int *data)
{
    if (!data)
        return;

    for (int row = 0; row < LEVEL_HEIGHT; ++row)
    {
        for (int col = 0; col < LEVEL_WIDTH; ++col)
        {
            bool border = (row == 0) || (row == LEVEL_HEIGHT - 1) || (col == 0) || (col == LEVEL_WIDTH - 1);

            data[row * LEVEL_WIDTH + col] = border ? 1u : 2u;
        }
    }
}

bool rectOverlap(const Rectangle &a, const Rectangle &b)
{
    bool overlapX = (a.x + a.width > b.x) && (a.x < b.x + b.width);
    bool overlapY = (a.y + a.height > b.y) && (a.y < b.y + b.height);
    return overlapX && overlapY;
}

int   gVirusMovePhase = 0;
float gVirusMoveTimer = 0.0f;

// L3
void SpawnVirusMove(Entity *virusMove, Vector2 ballPos)
{
    if (!virusMove)
        return;

    float   d = VIRUS_MOVE_OFFSET_DISTANCE;
    Vector2 p = ballPos;

    if (gVirusMovePhase == 0)
    {
        p.y += d;
    }
    else if (gVirusMovePhase == 1)
    {
        p.x += d;
    }
    else if (gVirusMovePhase == 2)
    {
        p.y -= d;
    }
    else
    {
        p.x -= d;
    }

    virusMove->setPosition(p);
    virusMove->setActive(true);

    gVirusMovePhase += 1;
    if (gVirusMovePhase >= 4)
    {
        gVirusMovePhase = 0;
    }
}

} // namespace

Level3Scene::Level3Scene()
    : Scene(), mMap(nullptr), mBall(nullptr), mLevelData(nullptr), mPendingReset(false), mResetCountdown(0.0f),
      mGameOverPlayed(false), mTimeLeft(0.0f), mPendingWin(false), mWinPlayed(false)
      // L3
      ,
      mVirusMove(nullptr)
{
}

Level3Scene::Level3Scene(Vector2 origin, const char *bgHexCode)
    : Scene(origin, bgHexCode), mMap(nullptr), mBall(nullptr), mLevelData(nullptr), mPendingReset(false),
      mResetCountdown(0.0f), mGameOverPlayed(false), mTimeLeft(0.0f), mPendingWin(false), mWinPlayed(false)
      // L3
      ,
      mVirusMove(nullptr)
{
}

Level3Scene::~Level3Scene()
{
    shutdown();
}

void Level3Scene::initialise()
{
    mGameState.nextSceneID = -1;

    mPendingReset   = false;
    mResetCountdown = 0.0f;
    mGameOverPlayed = false;

    mPendingWin = false;
    mWinPlayed  = false;
    //
    mTimeLeft = 110.0f;

    mSfxGameOver = LoadSound("assets/audios/game_over.mp3");
    mSfxWin      = LoadSound("assets/audios/win.mp3");
    mSfxGet      = LoadSound("assets/audios/get.mp3");
    // L3
    mSfxHit = LoadSound("assets/audios/hit.mp3");

    if (gScore > 420)
    {
        gScore = 420;
    };

    mLevelData = new unsigned int[LEVEL_WIDTH * LEVEL_HEIGHT];
    buildLevelData(mLevelData);

    mGameState.bgm = LoadMusicStream("assets/audios/gaming.mp3");
    PlayMusicStream(mGameState.bgm);

    mMap = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, mLevelData, "assets/wood.png", TILE_SIZE, 1, 1, mOrigin);

    Texture2D tempBallTex = LoadTexture("assets/ball_spritesheet.png");
    float     frameWidth  = (float)tempBallTex.width / (float)BALL_ATLAS_COLS;
    float     frameHeight = (float)tempBallTex.height / (float)BALL_ATLAS_ROWS;

    Vector2 ballScale = {frameWidth * BALL_SCALE_FACTOR, frameHeight * BALL_SCALE_FACTOR};
    UnloadTexture(tempBallTex);

    std::vector<int> frames = {1, 2};

    mBall = new Entity(mOrigin, ballScale, "assets/ball_spritesheet.png", ATLAS,
                       {(float)BALL_ATLAS_ROWS, (float)BALL_ATLAS_COLS}, frames);

    mBall->setSpeed(BALL_SPEED);
    mBall->setFrameSpeed(BALL_ANIM_FPS);
    mBall->setColliderScale(0.9f);

    mGameState.camera.target   = mBall->getPosition();
    mGameState.camera.offset   = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom     = 1.0f;

    InitCandies(mOrigin);

    // L2
    InitL2Extras(mOrigin);

    Texture2D tempVirusTex = LoadTexture("assets/virus2.png");
    float     vW           = (float)tempVirusTex.width;
    float     vH           = (float)tempVirusTex.height;

    Vector2 virusScale = {vW * VIRUS_MOVE_SCALE, vH * VIRUS_MOVE_SCALE};
    UnloadTexture(tempVirusTex);

    mVirusMove = new Entity(mOrigin, virusScale, "assets/virus2.png");
    mVirusMove->setSpeed((int)(BALL_SPEED * VIRUS_MOVE_SPEED_FACTOR));
    mVirusMove->setColliderScale(VIRUS_MOVE_COLLIDER_SCALE);
    mVirusMove->setActive(false);

    gVirusMovePhase = 0;
    gVirusMoveTimer = 0.0f;
}

void Level3Scene::update(float dt)
{

    if (mPendingReset)
    {
        if (!mGameOverPlayed)
        {
            StopMusicStream(mGameState.bgm);
            PlaySound(mSfxGameOver);
            mGameOverPlayed = true;
        }

        mResetCountdown -= dt;
        if (mResetCountdown <= 0.0f)
        {
            mGameState.nextSceneID = 0;
        }
        return;
    }

    if (mPendingWin)
    {
        if (!mWinPlayed)
        {
            StopMusicStream(mGameState.bgm);
            PlaySound(mSfxWin);
            mWinPlayed = true;
        }

        mResetCountdown -= dt;
        if (mResetCountdown <= 0.0f)
        {
            mGameState.nextSceneID = 0;
        }
        return;
    }

    UpdateMusicStream(mGameState.bgm);

    mTimeLeft -= dt;
    if (mTimeLeft <= 0.0f)
    {
        mResetCountdown = 3.0f;
        if (gScore >= LEVEL3_SCORE_LINE)
        {
            mPendingWin = true;
        }
        else
        {
            mPendingReset = true;
        }
        return;
    }

    // L2
    UpdateLightBalls(dt);

    ::UpdateStaticVirusesForLevel3(dt);

    if (!mBall)
        return;

    mBall->updateWithPlayerInput(dt);

    float halfMapW = (LEVEL_WIDTH * TILE_SIZE) * 0.5f;
    float halfMapH = (LEVEL_HEIGHT * TILE_SIZE) * 0.5f;

    float left   = mOrigin.x - halfMapW;
    float right  = mOrigin.x + halfMapW;
    float top    = mOrigin.y - halfMapH;
    float bottom = mOrigin.y + halfMapH;

    Vector2 pos  = mBall->getPosition();
    Vector2 size = mBall->getColliderDimensions();

    float halfW = size.x * 0.5f;
    float halfH = size.y * 0.5f;

    float minX = left + halfW;
    float maxX = right - halfW;
    float minY = top + halfH;
    float maxY = bottom - halfH;

    if (pos.x < minX)
        pos.x = minX;
    if (pos.x > maxX)
        pos.x = maxX;
    if (pos.y < minY)
        pos.y = minY;
    if (pos.y > maxY)
        pos.y = maxY;

    Rectangle ballRect;
    ballRect.x      = pos.x - halfW;
    ballRect.y      = pos.y - halfH;
    ballRect.width  = size.x;
    ballRect.height = size.y;

    int minCol = (int)floor((ballRect.x - left) / TILE_SIZE);
    int maxCol = (int)floor((ballRect.x + ballRect.width - left) / TILE_SIZE);
    int minRow = (int)floor((ballRect.y - top) / TILE_SIZE);
    int maxRow = (int)floor((ballRect.y + ballRect.height - top) / TILE_SIZE);

    if (minCol < 0)
        minCol = 0;
    if (maxCol >= LEVEL_WIDTH)
        maxCol = LEVEL_WIDTH - 1;
    if (minRow < 0)
        minRow = 0;
    if (maxRow >= LEVEL_HEIGHT)
        maxRow = LEVEL_HEIGHT - 1;

    for (int row = minRow; row <= maxRow; ++row)
    {
        for (int col = minCol; col <= maxCol; ++col)
        {
            unsigned int tile = mLevelData[row * LEVEL_WIDTH + col];

            if (tile != 1u)
                continue;

            Rectangle tileRect;
            tileRect.x      = left + col * TILE_SIZE;
            tileRect.y      = top + row * TILE_SIZE;
            tileRect.width  = TILE_SIZE;
            tileRect.height = TILE_SIZE;

            if (!rectOverlap(ballRect, tileRect))
                continue;

            mPendingReset   = true;
            mResetCountdown = 3.0f;
            return;
        }
    }

    mBall->setPosition(pos);

    Rectangle finalRect = mBall->getColliderRect();

    {
        bool hitVirus = false;
        UpdateCandiesAndViruses(finalRect, gScore, hitVirus, mSfxGet, mSfxHit);
        if (hitVirus)
        {
            mPendingReset   = true;
            mResetCountdown = 3.0f;
            return;
        }
    }

    // L2
    UpdateLightBallInteractions(finalRect, gScore, mSfxGet);

    // L3
    if (mVirusMove)
    {
        gVirusMoveTimer += dt;

        if (!mVirusMove->isActive())
        {
            if (gVirusMoveTimer >= VIRUS_MOVE_FIRST_SPAWN_TIME)
            {
                SpawnVirusMove(mVirusMove, mBall->getPosition());
                gVirusMoveTimer = 0.0f;
            }
        }
        else
        {
            if (gVirusMoveTimer >= VIRUS_MOVE_RESPAWN_INTERVAL)
            {
                SpawnVirusMove(mVirusMove, mBall->getPosition());
                gVirusMoveTimer = 0.0f;
            }

            Vector2 vp = mVirusMove->getPosition();
            Vector2 bp = mBall->getPosition();

            Vector2 dir;
            dir.x = bp.x - vp.x;
            dir.y = bp.y - vp.y;

            mVirusMove->setMovement(dir);
            mVirusMove->normaliseMovement();
            mVirusMove->update(dt);

            if (mBall->isCollidingWith(*mVirusMove))
            {
                gScore -= 40;
                PlaySound(mSfxHit);
                gVirusMoveTimer = 0.0f;
                SpawnVirusMove(mVirusMove, mBall->getPosition());
            }
        }
    }

    Vector2 target = mBall->getPosition();
    Vector2 diff   = {target.x - mGameState.camera.target.x, target.y - mGameState.camera.target.y};

    mGameState.camera.target.x += diff.x * 0.10f;
    mGameState.camera.target.y += diff.y * 0.10f;

    float halfViewW = (1400 / mGameState.camera.zoom) * 0.5f;
    float halfViewH = (900 / mGameState.camera.zoom) * 0.5f;

    float camMinX = left + halfViewW;
    float camMaxX = right - halfViewW;
    float camMinY = top + halfViewH;
    float camMaxY = bottom - halfViewH;

    if (right - left >= 2.0f * halfViewW)
    {
        mGameState.camera.target.x = fclamp(mGameState.camera.target.x, camMinX, camMaxX);
    }
    else
    {
        mGameState.camera.target.x = (left + right) * 0.5f;
    }

    if (bottom - top >= 2.0f * halfViewH)
    {
        mGameState.camera.target.y = fclamp(mGameState.camera.target.y, camMinY, camMaxY);
    }
    else
    {
        mGameState.camera.target.y = (top + bottom) * 0.5f;
    }
}

void Level3Scene::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    BeginMode2D(mGameState.camera);
    if (mMap)
        mMap->render();

    DrawCandies();

    if (mBall)
        mBall->render();

    DrawViruses();

    // L2
    DrawLightBalls();

    // L3
    if (mVirusMove && mVirusMove->isActive())
    {
        mVirusMove->render();
    }
    EndMode2D();

    const char *sLabel = "Score:";
    int         fs     = 24;
    int         w      = MeasureText(sLabel, fs);
    DrawText(sLabel, 20, 20, fs, WHITE);
    DrawText(TextFormat("%d", gScore), 20 + w + 10, 20, fs, WHITE);

    const char *tLabel = "Time Left:";
    DrawText(tLabel, 20, 50, fs, WHITE);
    DrawText(TextFormat("%d", (int)mTimeLeft), 150, 50, fs, WHITE);

    const char *lineLabel = "L2 Score Line:";
    int         lw        = MeasureText(lineLabel, fs);
    DrawText(lineLabel, 20, 80, fs, WHITE);
    DrawText(TextFormat("%d", LEVEL3_SCORE_LINE), 20 + lw + 10, 80, fs, WHITE);

    if (mPendingReset)
    {
        const char *msg      = "You Lose";
        const int   fontSize = 64;
        int         textW    = MeasureText(msg, fontSize);
        int         x        = (1400 - textW) / 2;
        int         y        = (900 - fontSize) / 2;
        DrawText(msg, x, y, fontSize, WHITE);
    }
    if (mPendingWin)
    {
        const char *msg      = "YOU WIN";
        const int   fontSize = 64;
        const int   textW    = MeasureText(msg, fontSize);
        const int   x        = (1400 - textW) / 2;
        const int   y        = (900 - fontSize) / 2;
        DrawText(msg, x, y, fontSize, WHITE);
    }
}

void Level3Scene::shutdown()
{
    if (mBall)
    {
        delete mBall;
        mBall = nullptr;
    }

    // L3
    if (mVirusMove)
    {
        delete mVirusMove;
        mVirusMove = nullptr;
    }

    if (mMap)
    {
        delete mMap;
        mMap = nullptr;
    }

    if (mLevelData)
    {
        delete[] mLevelData;
        mLevelData = nullptr;
    }

    UnloadSound(mSfxGameOver);
    UnloadSound(mSfxWin);
    UnloadSound(mSfxGet);
    UnloadSound(mSfxHit);

    ShutdownCandies();

    // L2
    ShutdownL2Extras();

    StopMusicStream(mGameState.bgm);
    UnloadMusicStream(mGameState.bgm);
}
