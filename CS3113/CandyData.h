
// CS3113/CandyData.h
#ifndef CANDYDATA_H
#define CANDYDATA_H

// 假设随机糖果数量固定为 100
static constexpr int RANDOM_CANDY_COUNT = 100;

// 三个数组由 CandyData.cpp 提供
extern const float CANDY_POS_X[RANDOM_CANDY_COUNT];
extern const float CANDY_POS_Y[RANDOM_CANDY_COUNT];
extern const float CANDY_ANGLE[RANDOM_CANDY_COUNT];

extern const float VIRUS_POS_X[20];
extern const float VIRUS_POS_Y[20];

#endif // CANDYDATA_H
