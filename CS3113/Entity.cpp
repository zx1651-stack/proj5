/**
* Author: Zirui Xu
* Assignment: Swallow
* Date due: 2025-12-05, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Entity.h"

static Rectangle getUVRectangle(const Texture2D *texture, int index, int rows, int cols)
{
    float uCoord = (float)(index % cols) / (float)cols;
    uCoord *= texture->width;

    float vCoord = (float)(index / cols) / (float)rows;
    vCoord *= texture->height;

    float sliceWidth  = texture->width / (float)cols;
    float sliceHeight = texture->height / (float)rows;

    Rectangle rect;
    rect.x      = uCoord;
    rect.y      = vCoord;
    rect.width  = sliceWidth;
    rect.height = sliceHeight;
    return rect;
}

Entity::Entity()
{
    mPosition.x = 0.0f;
    mPosition.y = 0.0f;

    mMovement.x = 0.0f;
    mMovement.y = 0.0f;

    mSpeed = 0;

    mScale.x = 0.0f;
    mScale.y = 0.0f;

    mTexture.id  = 0;
    mTextureType = SINGLE;

    mSpriteSheetDimensions.x = 1.0f;
    mSpriteSheetDimensions.y = 1.0f;

    mAnimationFrames.clear();
    mFrameSpeed        = 1;
    mCurrentFrameIndex = 0;
    mAnimationTime     = 0.0f;

    mColliderDimensions.x = 0.0f;
    mColliderDimensions.y = 0.0f;

    mIsActive = false;
}

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath)
{
    mPosition = position;

    mMovement.x = 0.0f;
    mMovement.y = 0.0f;

    mSpeed = 0;

    mScale = scale;

    mTexture     = LoadTexture(textureFilepath);
    mTextureType = SINGLE;

    mSpriteSheetDimensions.x = 1.0f;
    mSpriteSheetDimensions.y = 1.0f;

    mAnimationFrames.clear();
    mFrameSpeed        = 1;
    mCurrentFrameIndex = 0;
    mAnimationTime     = 0.0f;

    mColliderDimensions = scale;

    mIsActive = true;
}

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath, TextureType textureType,
               Vector2 spriteSheetDimensions, std::vector<int> animationFrames)
{
    mPosition = position;

    mMovement.x = 0.0f;
    mMovement.y = 0.0f;

    mSpeed = 0;

    mScale = scale;

    mTexture     = LoadTexture(textureFilepath);
    mTextureType = textureType;

    mSpriteSheetDimensions = spriteSheetDimensions;

    mAnimationFrames   = animationFrames;
    mFrameSpeed        = 8;
    mCurrentFrameIndex = 0;
    mAnimationTime     = 0.0f;

    mColliderDimensions = scale;

    mIsActive = true;
}

Entity::~Entity()
{
    if (mTexture.id != 0)
    {
        UnloadTexture(mTexture);
        mTexture.id = 0;
    }
}

void Entity::update(float deltaTime)
{
    if (!mIsActive)
    {
        return;
    }

    mPosition.x += mMovement.x * (float)mSpeed * deltaTime;
    mPosition.y += mMovement.y * (float)mSpeed * deltaTime;

    if (mTextureType == ATLAS && !mAnimationFrames.empty())
    {
        mAnimationTime += deltaTime;

        float secondsPerFrame = 1.0f / (float)mFrameSpeed;

        if (mAnimationTime >= secondsPerFrame)
        {
            mAnimationTime = 0.0f;

            mCurrentFrameIndex++;
            int frameCount = (int)mAnimationFrames.size();

            if (frameCount > 0)
            {
                mCurrentFrameIndex %= frameCount;
            }
            else
            {
                mCurrentFrameIndex = 0;
            }
        }
    }
}

Rectangle Entity::getTextureSourceRectangle() const
{
    if (mTextureType == SINGLE || mAnimationFrames.empty())
    {
        Rectangle rect;
        rect.x      = 0.0f;
        rect.y      = 0.0f;
        rect.width  = (float)mTexture.width;
        rect.height = (float)mTexture.height;
        return rect;
    }

    int rows = (int)mSpriteSheetDimensions.x;
    int cols = (int)mSpriteSheetDimensions.y;

    int index = mAnimationFrames[mCurrentFrameIndex];
    return getUVRectangle(&mTexture, index, rows, cols);
}

void Entity::render() const
{
    if (!mIsActive)
    {
        return;
    }

    Rectangle source = getTextureSourceRectangle();

    Rectangle dest;
    dest.x      = mPosition.x;
    dest.y      = mPosition.y;
    dest.width  = mScale.x;
    dest.height = mScale.y;

    Vector2 origin;
    origin.x = mScale.x * 0.5f;
    origin.y = mScale.y * 0.5f;

    DrawTexturePro(mTexture, source, dest, origin, 0.0f, WHITE);
}

void Entity::resetMovement()
{
    mMovement.x = 0.0f;
    mMovement.y = 0.0f;
}

void Entity::moveLeft()
{
    mMovement.x = -1.0f;
}

void Entity::moveRight()
{
    mMovement.x = 1.0f;
}

void Entity::moveUp()
{
    mMovement.y = -1.0f;
}

void Entity::moveDown()
{
    mMovement.y = 1.0f;
}

void Entity::normaliseMovement()
{
    Normalise(&mMovement);
}

void Entity::updateWithPlayerInput(float deltaTime)
{
    if (!mIsActive)
    {
        return;
    }

    resetMovement();

    if (IsKeyDown(KEY_A))
    {
        moveLeft();
    }
    if (IsKeyDown(KEY_D))
    {
        moveRight();
    }
    if (IsKeyDown(KEY_W))
    {
        moveUp();
    }
    if (IsKeyDown(KEY_S))
    {
        moveDown();
    }

    update(deltaTime);
}

Vector2 Entity::getPosition() const
{
    return mPosition;
}

void Entity::setPosition(Vector2 position)
{
    mPosition = position;
}

Vector2 Entity::getMovement() const
{
    return mMovement;
}

void Entity::setMovement(Vector2 movement)
{
    mMovement = movement;
}

int Entity::getSpeed() const
{
    return mSpeed;
}

void Entity::setSpeed(int speed)
{
    mSpeed = speed;
}

Vector2 Entity::getScale() const
{
    return mScale;
}

void Entity::setScale(Vector2 scale)
{
    mScale = scale;
}

bool Entity::isActive() const
{
    return mIsActive;
}

void Entity::setActive(bool isActive)
{
    mIsActive = isActive;
}

void Entity::setAnimationFrames(const std::vector<int> &frames)
{
    mAnimationFrames   = frames;
    mCurrentFrameIndex = 0;
    mAnimationTime     = 0.0f;
}

void Entity::setFrameSpeed(int framesPerSecond)
{
    mFrameSpeed = framesPerSecond;
}

Vector2 Entity::getColliderDimensions() const
{
    return mColliderDimensions;
}

void Entity::setColliderDimensions(Vector2 dims)
{
    mColliderDimensions = dims;
}

void Entity::setColliderScale(float factor)
{
    mColliderDimensions.x = mScale.x * factor;
    mColliderDimensions.y = mScale.y * factor;
}

Rectangle Entity::getColliderRect() const
{
    float halfW = mColliderDimensions.x * 0.5f;
    float halfH = mColliderDimensions.y * 0.5f;

    Rectangle rect;
    rect.x      = mPosition.x - halfW;
    rect.y      = mPosition.y - halfH;
    rect.width  = mColliderDimensions.x;
    rect.height = mColliderDimensions.y;
    return rect;
}

bool Entity::isCollidingWith(const Entity &other) const
{
    if (!mIsActive || !other.mIsActive)
    {
        return false;
    }

    Rectangle a = getColliderRect();
    Rectangle b = other.getColliderRect();

    bool overlapX = (a.x + a.width > b.x) && (a.x < b.x + b.width);
    bool overlapY = (a.y + a.height > b.y) && (a.y < b.y + b.height);

    return overlapX && overlapY;
}
