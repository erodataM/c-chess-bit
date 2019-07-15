#ifndef CHESS_UTILS_H
#define CHESS_UTILS_H

#include <random>
#include <vector>

class Utils
{
public:
    unsigned long long one = 1;
    unsigned long long ones[64];
    int tab64[64];
    int tab120[120];
    int pawnMoves[2];
    int pawnCaptures[2][2][2];
    std::vector<int> pawnCapturesCache[2][64];
    std::vector<unsigned long long> pawnCapturesCachePosition[2][64];
    int knightMoves[8][2];
    std::vector<int> knightMovesCache[64];
    std::vector<unsigned long long> knightMovesCachePosition[64];
    int bishopMoves[4][2];
    std::vector<int> bishopMovesCache[64][4];
    std::vector<unsigned long long> bishopMovesCachePosition[64][4];
    int rookMoves[4][2];
    std::vector<int> rookMovesCache[64][4];
    std::vector<unsigned long long> rookMovesCachePosition[64][4];
    int queenMoves[8][2];
    std::vector<int> queenMovesCache[64][8];
    std::vector<unsigned long long> queenMovesCachePosition[64][8];
    std::vector<int> kingMovesCache[64];
    std::vector<unsigned long long> kingMovesCachePosition[64];

    unsigned long long shortCastlesCache[2];
    unsigned long long longCastlesCache[2];

    Utils();
    unsigned long long getRand();
    static void setBit(unsigned long long, unsigned long long*);
    static void unsetBit(unsigned long long, unsigned long long*);
    static bool isBit(unsigned long long, unsigned long long);
    static bool isPawnPromote(bool, int);
    static bool isPawnFirstMove(bool, int);
    bool isInBoard(int, int);

private:
    std::random_device rd;
};
#endif //CHESS_UTILS_H
