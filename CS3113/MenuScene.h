#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"

class MenuScene : public Scene
{
  public:
    MenuScene();
    MenuScene(Vector2 origin, const char *bgHex = "#000000");
    ~MenuScene();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
