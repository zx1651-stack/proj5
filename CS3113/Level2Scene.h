#ifndef LEVEL2SCENE_H
#define LEVEL2SCENE_H

#include "Scene.h"

class Map;
class Entity;

class Level2Scene : public Scene
{
  public:
    Level2Scene();
    Level2Scene(Vector2 origin, const char *bgHexCode);
    ~Level2Scene();

    void initialise() override;
    void update(float dt) override;
    void render() override;
    void shutdown() override;

  private:
    Map          *mMap;
    Entity       *mBall;
    unsigned int *mLevelData;

    bool  mPendingReset;
    float mResetCountdown;
    bool  mGameOverPlayed;

    float mTimeLeft;
    bool  mPendingWin;
    bool  mWinPlayed;

    Sound mSfxGameOver;
    Sound mSfxWin;
    Sound mSfxGet; // [L2] 光球吃到时的 get.mp3
    Sound mSfxHit; // [L3] VirusMove 命中时的 hit.mp3

    // [L3] VirusMove 实体（仅 L3 使用，L1/L2 版本删除）
    Entity *mVirusMove;
};

#endif
