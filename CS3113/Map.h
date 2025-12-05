#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include <vector>

class Map
{
  private:
    int                 mMapColumns;
    int                 mMapRows;
    const unsigned int *mLevelData;

    Texture2D              mTextureAtlas;
    int                    mTextureColumns;
    int                    mTextureRows;
    std::vector<Rectangle> mTextureAreas;

    float   mTileSize;
    Vector2 mOrigin;

    float mLeftBoundary;
    float mRightBoundary;
    float mTopBoundary;
    float mBottomBoundary;

  public:
    Map(int mapColumns, int mapRows, const unsigned int *levelData, const char *textureFilePath, float tileSize,
        int textureColumns, int textureRows, Vector2 origin);

    ~Map();

    void build();
    void render();
    bool isSolidTileAt(Vector2 point, float *xOverlap, float *yOverlap);
};

#endif
