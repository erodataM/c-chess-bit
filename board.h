#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "utils.h"
#include "move.h"
#include <string>
#include <vector>

class Board
{
public:
    explicit Board(std::string);
    Utils* utils;
    bool trait;
    int pep;
    int kingPos[2];
    bool shortCastles[2];
    bool longCastles[2];
    unsigned long long pieces[2][6];
    unsigned long long white;
    unsigned long long black;
    unsigned long long board;
    unsigned long long zobristKey;

    bool isInCheck();
    void display();
    std::vector<std::vector<Move>> getMoves();
    void makeMove(std::vector<Move>, bool);

private:
    unsigned long long zobristBoard[64][13];
    unsigned long long zobristTrait[2];
    unsigned long long zobristPep[65];

    void initZobrist();
    void init();
    int getPieceAt(int);
    void applyMove(std::vector<Move>*, std::vector<std::vector<Move>>*);
    void applyPawnMoves(int, std::vector<std::vector<Move>>*);
    void applyKnightMoves(int, std::vector<std::vector<Move>>*);
    void applyBishopMoves(int, std::vector<std::vector<Move>>*);
    void applyRookMoves(int, std::vector<std::vector<Move>>*);
    void applyQueenMoves(int, std::vector<std::vector<Move>>*);
    void applyKingMoves(int, std::vector<std::vector<Move>>*);
};

#endif //CHESS_BOARD_H
