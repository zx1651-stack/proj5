/**
* Author: Zirui Xu
* Assignment: Swallow
* Date due: 2025-12-05, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Map.h"
#include "cs3113.h"
#include <cmath>

Map::Map(int mapColumns, int mapRows, const unsigned int *levelData, const char *textureFilePath, float tileSize,
         int textureColumns, int textureRows, Vector2 origin)
    : mMapColumns{mapColumns}, mMapRows{mapRows}, mLevelData{levelData}, mTextureAtlas{LoadTexture(textureFilePath)},
      mTextureColumns{textureColumns}, mTextureRows{textureRows}, mTileSize{tileSize}, mOrigin{origin},
      mLeftBoundary{0}, mRightBoundary{0}, mTopBoundary{0}, mBottomBoundary{0}
{
    build();
}

Map::~Map()
{
    if (mTextureAtlas.id > 0)
        UnloadTexture(mTextureAtlas);
}

void Map::build()
{
    mLeftBoundary   = mOrigin.x - (mMapColumns * mTileSize) / 2.0f;
    mRightBoundary  = mOrigin.x + (mMapColumns * mTileSize) / 2.0f;
    mTopBoundary    = mOrigin.y - (mMapRows * mTileSize) / 2.0f;
    mBottomBoundary = mOrigin.y + (mMapRows * mTileSize) / 2.0f;

    mTextureAreas.clear();
    for (int r = 0; r < mTextureRows; ++r)
    {
        for (int c = 0; c < mTextureColumns; ++c)
        {
            Rectangle uv = {(float)c * (mTextureAtlas.width / mTextureColumns),
                            (float)r * (mTextureAtlas.height / mTextureRows),
                            (float)mTextureAtlas.width / mTextureColumns, (float)mTextureAtlas.height / mTextureRows};
            mTextureAreas.push_back(uv);
        }
    }
}

void Map::render()
{
    for (int row = 0; row < mMapRows; ++row)
    {
        for (int col = 0; col < mMapColumns; ++col)
        {
            unsigned int tile = mLevelData[row * mMapColumns + col];

            if (tile == 0)
                continue;

            int atlasIndex = (int)tile - 1;
            if (atlasIndex < 0 || atlasIndex >= (int)mTextureAreas.size())
            {
                atlasIndex = 0;
            }

            Rectangle dst = {mLeftBoundary + col * mTileSize, mTopBoundary + row * mTileSize, mTileSize, mTileSize};

            Color tint = WHITE;
            if (tile == 2u)
            {
                tint = Fade(WHITE, 0.15f);
            }

            DrawTexturePro(mTextureAtlas, mTextureAreas[atlasIndex], dst, {0.0f, 0.0f}, 0.0f, tint);
        }
    }
}

bool Map::isSolidTileAt(Vector2 point, float *xOverlap, float *yOverlap)
{
    if (xOverlap)
        *xOverlap = 0.0f;
    if (yOverlap)
        *yOverlap = 0.0f;

    if (point.x < mLeftBoundary || point.x > mRightBoundary || point.y < mTopBoundary || point.y > mBottomBoundary)
        return false;

    int tileXIndex = (int)std::floor((point.x - mLeftBoundary) / mTileSize);
    int tileYIndex = (int)std::floor((point.y - mTopBoundary) / mTileSize);

    if (tileXIndex < 0 || tileXIndex >= mMapColumns || tileYIndex < 0 || tileYIndex >= mMapRows)
        return false;

    int tile = (int)mLevelData[tileYIndex * mMapColumns + tileXIndex];
    if (tile == 0)
        return false;

    float tileCentreX = mLeftBoundary + tileXIndex * mTileSize + mTileSize * 0.5f;
    float tileCentreY = mTopBoundary + tileYIndex * mTileSize + mTileSize * 0.5f;

    float half = mTileSize * 0.5f;
    float xo   = half - std::fabs(point.x - tileCentreX);
    float yo   = half - std::fabs(point.y - tileCentreY);

    if (xOverlap)
        *xOverlap = (xo > 0.0f) ? xo : 0.0f;
    if (yOverlap)
        *yOverlap = (yo > 0.0f) ? yo : 0.0f;

    return true;
}
