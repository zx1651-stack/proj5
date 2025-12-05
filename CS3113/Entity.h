#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"
#include "raylib.h"
#include <vector>

enum TextureType
{
    SINGLE,
    ATLAS
};

class Entity
{
  public:
    Entity();
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath);
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath, TextureType textureType,
           Vector2 spriteSheetDimensions, std::vector<int> animationFrames);

    ~Entity();

    void update(float deltaTime);
    void render() const;

    void resetMovement();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void normaliseMovement();

    Vector2 getPosition() const;
    void    setPosition(Vector2 position);

    Vector2 getMovement() const;
    void    setMovement(Vector2 movement);

    int  getSpeed() const;
    void setSpeed(int speed);

    Vector2 getScale() const;
    void    setScale(Vector2 scale);

    bool isActive() const;
    void setActive(bool isActive);

    void setAnimationFrames(const std::vector<int> &frames);
    void setFrameSpeed(int framesPerSecond);

    Vector2 getColliderDimensions() const;
    void    setColliderDimensions(Vector2 dims);

    void setColliderScale(float factor);

    Rectangle getColliderRect() const;

    bool isCollidingWith(const Entity &other) const;
    void updateWithPlayerInput(float deltaTime);

  private:
    Vector2 mPosition;
    Vector2 mMovement;
    int     mSpeed;
    Vector2 mScale;

    Texture2D   mTexture;
    TextureType mTextureType;

    Vector2 mSpriteSheetDimensions;

    std::vector<int> mAnimationFrames;
    int              mFrameSpeed;
    int              mCurrentFrameIndex;
    float            mAnimationTime;

    Vector2 mColliderDimensions;

    bool mIsActive;

    Rectangle getTextureSourceRectangle() const;
};

#endif // ENTITY_H
