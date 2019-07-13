#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include "utils.h"

class Move {
public:
    Move(int, bool, int, unsigned long long, bool);
    Move(std::string, int, int);
    Move(std::string, int, int, bool);
    int pieceType;
    bool trait;
    int position;
    unsigned long long value;
    int pep[2];
    int king[2];
    std::string type;
    bool doit;
};

#endif //CHESS_MOVE_H
