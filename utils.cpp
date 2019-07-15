#include "utils.h"

#include <random>
#include <iostream>

Utils::Utils()
{
    this->shortCastlesCache[0] = 0;
    this->longCastlesCache[0] = 0;
    this->shortCastlesCache[1] = 0;
    this->longCastlesCache[1] = 0;

    for (int i = 0; i < 64; ++i) {
        this->ones[i] = this->one << i;
        if (i == 61 || i == 62) {
            Utils::setBit(this->ones[1], &this->shortCastlesCache[1]);
        }
        if (i == 59 || i == 58 || i == 57) {
            Utils::setBit(this->ones[1], &this->longCastlesCache[1]);
        }
        if (i == 5 || i == 6) {
            Utils::setBit(this->ones[1], &this->shortCastlesCache[0]);
        }
        if (i == 3 || i == 2 || i == 1) {
            Utils::setBit(this->ones[1], &this->longCastlesCache[0]);
        }
    }

    this->pawnMoves[0] = 8;
    this->pawnMoves[1] = -8;

    this->pawnCaptures[0][0][0] = 11;
    this->pawnCaptures[0][0][1] = 9;
    this->pawnCaptures[0][1][0] = 9;
    this->pawnCaptures[0][1][1] = 7;

    this->pawnCaptures[1][0][0] = -11;
    this->pawnCaptures[1][0][1] = -9;
    this->pawnCaptures[1][1][0] = -9;
    this->pawnCaptures[1][1][1] = -7;

    this->knightMoves[0][0] = -21;
    this->knightMoves[0][1] = -17;
    this->knightMoves[1][0] = -19;
    this->knightMoves[1][1] = -15;
    this->knightMoves[2][0] = -12;
    this->knightMoves[2][1] = -10;
    this->knightMoves[3][0] = -8;
    this->knightMoves[3][1] = -6;
    this->knightMoves[4][0] = 21;
    this->knightMoves[4][1] = 17;
    this->knightMoves[5][0] = 19;
    this->knightMoves[5][1] = 15;
    this->knightMoves[6][0] = 12;
    this->knightMoves[6][1] = 10;
    this->knightMoves[7][0] = 8;
    this->knightMoves[7][1] = 6;

    this->bishopMoves[0][0] = -11;
    this->bishopMoves[0][1] = -9;
    this->bishopMoves[1][0] = -9;
    this->bishopMoves[1][1] = -7;
    this->bishopMoves[2][0] = 11;
    this->bishopMoves[2][1] = 9;
    this->bishopMoves[3][0] = 9;
    this->bishopMoves[3][1] = 7;

    this->rookMoves[0][0] = -10;
    this->rookMoves[0][1] = -8;
    this->rookMoves[1][0] = -1;
    this->rookMoves[1][1] = -1;
    this->rookMoves[2][0] = 10;
    this->rookMoves[2][1] = 8;
    this->rookMoves[3][0] = 1;
    this->rookMoves[3][1] = 1;

    this->queenMoves[0][0] = -10;
    this->queenMoves[0][1] = -8;
    this->queenMoves[1][0] = -1;
    this->queenMoves[1][1] = -1;
    this->queenMoves[2][0] = 10;
    this->queenMoves[2][1] = 8;
    this->queenMoves[3][0] = 1;
    this->queenMoves[3][1] = 1;
    this->queenMoves[4][0] = -11;
    this->queenMoves[4][1] = -9;
    this->queenMoves[5][0] = -9;
    this->queenMoves[5][1] = -7;
    this->queenMoves[6][0] = 11;
    this->queenMoves[6][1] = 9;
    this->queenMoves[7][0] = 9;
    this->queenMoves[7][1] = 7;

    int j = 0;
    int k = 0;
    for (int i = 0; i < 120; ++i) {
        if (j == 0 || j == 9 || k == 0 || k == 1 || k == 10 || k == 11) {
            this->tab120[i] = 0;
        } else {
            this->tab120[i] = 1;
        }
        if (++j == 10) {
            j = 0;
            k++;
        }
    }

    int count = 0;
    for (int i = 0; i < 120; ++i) {
        if (this->tab120[i] == 1) {
            this->tab64[count++] = i;
        }
    }

    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                if (this->isInBoard(i, this->pawnCaptures[j][k][0])) {
                    this->pawnCapturesCache[j][i].push_back(i + this->pawnCaptures[j][k][1]);
                    this->pawnCapturesCachePosition[j][i].push_back(this->one << (i + this->pawnCaptures[j][k][1]));
                }
            }
        }
    }

    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (this->isInBoard(i, this->knightMoves[j][0])) {
                this->knightMovesCache[i].push_back(i + this->knightMoves[j][1]);
                this->knightMovesCachePosition[i].push_back(this->one << (i + this->knightMoves[j][1]));
            }
        }
    }

    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 4; ++j) {
            int k = 1;
            while (this->isInBoard(i, this->bishopMoves[j][0] * k)) {
                this->bishopMovesCache[i][j].push_back(i + this->bishopMoves[j][1] * k);
                this->bishopMovesCachePosition[i][j].push_back(this->one << (i + this->bishopMoves[j][1] * k));
                k++;
            }
        }
    }

    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 4; ++j) {
            int k = 1;
            while (this->isInBoard(i, this->rookMoves[j][0] * k)) {
                this->rookMovesCache[i][j].push_back(i + this->rookMoves[j][1] * k);
                this->rookMovesCachePosition[i][j].push_back(this->one << (i + this->rookMoves[j][1] * k));
                k++;
            }
        }
    }

    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 8; ++j) {
            int k = 1;
            while (this->isInBoard(i, this->queenMoves[j][0] * k)) {
                this->queenMovesCache[i][j].push_back(i + this->queenMoves[j][1] * k);
                this->queenMovesCachePosition[i][j].push_back(this->one << (i + this->queenMoves[j][1] * k));
                k++;
            }
        }
    }

    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (this->isInBoard(i, this->queenMoves[j][0])) {
                this->kingMovesCache[i].push_back(i + this->queenMoves[j][1]);
                this->kingMovesCachePosition[i].push_back(this->one << (i + this->queenMoves[j][1]));
            }
        }
    }
}

unsigned long long Utils::getRand()
{
    std::default_random_engine generator(this->rd());
    std::uniform_int_distribution<long long unsigned> distribution(0,0xFFFFFFFFFFFFFFFF);
    return distribution(generator);
}

void Utils::setBit(unsigned long long i, unsigned long long *number)
{
    *number |= i;
}

void Utils::unsetBit(unsigned long long i, unsigned long long *number)
{
    *number &= ~i;
}

bool Utils::isBit(unsigned long long i, unsigned long long number)
{
    return (number & i) == i;
}

bool Utils::isPawnPromote(bool trait, int i)
{
    return trait ? (i > 7 && i < 16) : (i > 47 && i < 56);
}

bool Utils::isPawnFirstMove(bool trait, int i)
{
    return trait ? (i > 47 && i < 56) : (i > 7 && i < 16);
}

bool Utils::isInBoard(int i, int l)
{
    return this->tab120[this->tab64[i] + l];
}

//  0  1  2  3  4  5 6   7
//  8  9 10 11 12 13 14 15
// 16 17 18 19 20 21 22 23
// 24 25 26 27 28 29 30 31
// 32 33 34 35 36 37 38 39
// 40 41 42 43 44 45 46 47
// 48 49 50 51 52 53 54 55
// 56 57 58 59 60 61 62 63

// 21 22 23 24 25 26 27 28
// 31 32 33 34 35 36 37 38
// 41 42 43 44 45 46 47 48
// 51 52 53 54 55 56 57 58
// 61 62 63 64 65 66 67 68
// 71 72 73 74 75 76 77 78
// 81 82 83 84 85 86 87 88
// 91 92 93 94 95 96 97 98

// 0 0 0 0 0 0 0 0 0 0 
// 0 0 0 0 0 0 0 0 0 0
// 0 1 1 1 1 1 1 1 1 0 
// 0 1 1 1 1 1 1 1 1 0 
// 0 1 1 1 1 1 1 1 1 0 
// 0 1 1 1 1 1 1 1 1 0 
// 0 1 1 1 1 1 1 1 1 0 
// 0 1 1 1 1 1 1 1 1 0 
// 0 1 1 1 1 1 1 1 1 0
// 0 1 1 1 1 1 1 1 1 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0

//   0   1   2   3   4   5   6   7   8   9 
//  10  11  12  13  14  15  16  17  18  19
//  20  21  22  23  24  25  26  27  28  29
//  30  31  32  33  34  35  36  37  38  39
//  40  41  42  43  44  45  46  47  48  49
//  50  51  52  53  54  55  56  57  58  59
//  60  61  62  63  64  65  66  67  68  69
//  70  71  72  73  74  75  76  77  78  79
//  80  81  82  83  84  85  86  87  88  89
//  90  91  92  93  94  95  96  97  98  99
// 100 101 102 103 104 105 106 107 108 109
// 110 111 112 113 114 115 116 117 118 119