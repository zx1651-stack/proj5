/**
* Author: Zirui Xu
* Assignment: Swallow
* Date due: 2025-12-05, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Level1Scene.h"
#include "CS3113/Level2Scene.h"
#include "CS3113/Level3Scene.h"
#include "CS3113/MenuScene.h"
#include "CS3113/cs3113.h"
#include "raylib.h"

int gScore = 0;

int main()
{
    const int screenWidth  = 1400;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Project 5 - Menu + L1 + L2 + L3");
    InitAudioDevice();
    SetTargetFPS(60);

    MenuScene *menu = new MenuScene({screenWidth * 0.5f, screenHeight * 0.5f}, "#000000");
    menu->initialise();

    Level1Scene *level1 = nullptr;
    Level2Scene *level2 = nullptr;
    Level3Scene *level3 = nullptr;

    bool inLevel1 = false;
    bool inLevel2 = false;
    bool inLevel3 = false;

    float previousTicks = (float)GetTime();

    while (!WindowShouldClose())
    {
        float currentTicks = (float)GetTime();
        float dt           = currentTicks - previousTicks;
        previousTicks      = currentTicks;

        BeginDrawing();

        if (!inLevel1 && !inLevel2 && !inLevel3)
        {
            menu->update(dt);
            menu->render();

            if (menu->getState().nextSceneID == 1)
            {
                delete menu;
                menu   = nullptr;
                level1 = new Level1Scene({screenWidth * 0.5f, screenHeight * 0.5f}, "#444444");
                level1->initialise();
                inLevel1 = true;
                inLevel2 = false;
                inLevel3 = false;
            }
        }
        else if (inLevel1)
        {
            level1->update(dt);
            level1->render();

            int next = level1->getState().nextSceneID;

            if (next == 0)
            {
                delete level1;
                level1   = nullptr;
                inLevel1 = false;
                menu     = new MenuScene({screenWidth * 0.5f, screenHeight * 0.5f}, "#000000");
                menu->initialise();
            }
            else if (next == 2)
            {
                delete level1;
                level1   = nullptr;
                inLevel1 = false;
                level2   = new Level2Scene({screenWidth * 0.5f, screenHeight * 0.5f}, "#333344");
                level2->initialise();
                inLevel2 = true;
                inLevel3 = false;
            }
        }
        else if (inLevel2)
        {
            level2->update(dt);
            level2->render();

            int next = level2->getState().nextSceneID;

            if (next == 0)
            {
                delete level2;
                level2   = nullptr;
                inLevel2 = false;
                menu     = new MenuScene({screenWidth * 0.5f, screenHeight * 0.5f}, "#000000");
                menu->initialise();
            }
            else if (next == 3)
            {
                delete level2;
                level2   = nullptr;
                inLevel2 = false;
                level3   = new Level3Scene({screenWidth * 0.5f, screenHeight * 0.5f}, "#223344");
                level3->initialise();
                inLevel3 = true;
            }
        }
        else if (inLevel3)
        {
            level3->update(dt);
            level3->render();

            int next = level3->getState().nextSceneID;

            if (next == 0)
            {
                delete level3;
                level3   = nullptr;
                inLevel3 = false;
                menu     = new MenuScene({screenWidth * 0.5f, screenHeight * 0.5f}, "#000000");
                menu->initialise();
            }
        }

        EndDrawing();
    }

    if (level1)
    {
        delete level1;
        level1 = nullptr;
    }
    if (level2)
    {
        delete level2;
        level2 = nullptr;
    }
    if (level3)
    {
        delete level3;
        level3 = nullptr;
    }
    if (menu)
    {
        delete menu;
        menu = nullptr;
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
