#ifndef SCENE_H
#define SCENE_H

#include "cs3113.h"

class Entity;
class Map;

struct GameState
{
    Entity  *xochitl = nullptr;
    Map     *map     = nullptr;
    Music    bgm{};
    Sound    jumpSound{};
    Camera2D camera{};
    int      nextSceneID = 0;
};

class Scene
{
  protected:
    GameState   mGameState{};
    Vector2     mOrigin{};
    const char *mBGColourHexCode = "#000000";

  public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);
    virtual ~Scene() = default;

    virtual void initialise()     = 0;
    virtual void update(float dt) = 0;
    virtual void render()         = 0;
    virtual void shutdown()       = 0;

    const GameState &getState() const
    {
        return mGameState;
    }
    GameState &getState()
    {
        return mGameState;
    }

    Vector2 getOrigin() const
    {
        return mOrigin;
    }
    const char *getBGColourHexCode() const
    {
        return mBGColourHexCode;
    }
};

#endif
