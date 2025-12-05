/**
* Author: Zirui Xu
* Assignment: Swallow
* Date due: 2025-12-05, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "MenuScene.h"
#include "raylib.h"

MenuScene::MenuScene() : Scene({0.0f}, "#000000")
{
}
MenuScene::MenuScene(Vector2 origin, const char *bgHex) : Scene(origin, bgHex)
{
}
MenuScene::~MenuScene()
{
    shutdown();
}

void MenuScene::initialise()
{
    mGameState.nextSceneID = -1;

    mGameState.bgm = LoadMusicStream("assets/audios/menu.mp3");
    SetMusicVolume(mGameState.bgm, 1.0f);
    PlayMusicStream(mGameState.bgm);
}

void MenuScene::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    if (IsKeyPressed(KEY_ENTER))
    {
        mGameState.nextSceneID = 1;
    }

    (void)deltaTime;
}

void MenuScene::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    const char *title   = "SWALLOW";
    const char *prompt1 = "Press ENTER to start";
    const char *prompt2 = "Use ASDW to move";
    const char *prompt3 = "Try to eat as much food as possible to win!";

    int titleSize  = 50;
    int promptSize = 25;

    int titleW = MeasureText(title, titleSize);
    int p1w    = MeasureText(prompt1, promptSize);
    int p2w    = MeasureText(prompt2, promptSize);
    int p3w    = MeasureText(prompt3, promptSize);

    DrawText(title, (int)(mOrigin.x - titleW / 2.0f), (int)(mOrigin.y - 50), titleSize, WHITE);
    DrawText(prompt1, (int)(mOrigin.x - p1w / 2.0f), (int)(mOrigin.y + 20), promptSize, WHITE);
    DrawText(prompt2, (int)(mOrigin.x - p2w / 2.0f), (int)(mOrigin.y + 60), promptSize, WHITE);
    DrawText(prompt3, (int)(mOrigin.x - p3w / 2.0f), (int)(mOrigin.y + 100), promptSize, WHITE);
}

void MenuScene::shutdown()
{
    StopMusicStream(mGameState.bgm);
    UnloadMusicStream(mGameState.bgm);
}
