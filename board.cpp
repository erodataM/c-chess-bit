#include "board.h"
#include "move.h"

#include <iostream>
#include <bitset>
#include <string>
#include <vector>

using namespace std;

Board::Board(string fen)
{
    this->utils = new Utils();
    std::string fenDiag = fen.substr(0, fen.find(' '));
    std::string fenTrait = fen.substr(fen.find(' ') + 1, 1);
    std::string fenCastles = fen.substr(fen.find(' ') + 3, 4);
    std::string fenPep = fen.substr(fen.find(' ') + 8);

    this->pieces[0][0] = 0;
    this->pieces[0][1] = 0;
    this->pieces[0][2] = 0;
    this->pieces[0][3] = 0;
    this->pieces[0][4] = 0;
    this->pieces[0][5] = 0;

    this->pieces[1][0] = 0;
    this->pieces[1][1] = 0;
    this->pieces[1][2] = 0;
    this->pieces[1][3] = 0;
    this->pieces[1][4] = 0;
    this->pieces[1][5] = 0;

    int count = 0;
    for (int i = 0; i < fenDiag.size(); ++i)
    {
        if (fenDiag.at(i) == 'p') {
            Utils::setBit(this->utils->ones[count], &this->pieces[0][0]);
            count++;
        } else if (fenDiag.at(i) == 'P') {
            Utils::setBit(this->utils->ones[count], &this->pieces[1][0]);
            count++;
        } else if (fenDiag.at(i) == 'n') {
            Utils::setBit(this->utils->ones[count], &this->pieces[0][1]);
            count++;
        } else if (fenDiag.at(i) == 'N') {
            Utils::setBit(this->utils->ones[count], &this->pieces[1][1]);
            count++;
        } else if (fenDiag.at(i) == 'b') {
            Utils::setBit(this->utils->ones[count], &this->pieces[0][2]);
            count++;
        } else if (fenDiag.at(i) == 'B') {
            Utils::setBit(this->utils->ones[count], &this->pieces[1][2]);
            count++;
        } else if (fenDiag.at(i) == 'r') {
            Utils::setBit(this->utils->ones[count], &this->pieces[0][3]);
            count++;
        } else if (fenDiag.at(i) == 'R') {
            Utils::setBit(this->utils->ones[count], &this->pieces[1][3]);
            count++;
        } else if (fenDiag.at(i) == 'q') {
            Utils::setBit(this->utils->ones[count], &this->pieces[0][4]);
            count++;
        } else if (fenDiag.at(i) == 'Q') {
            Utils::setBit(this->utils->ones[count], &this->pieces[1][4]);
            count++;
        } else if (fenDiag.at(i) == 'k') {
            Utils::setBit(this->utils->ones[count], &this->pieces[0][5]);
            this->kingPos[0] = count;
            count++;
        } else if (fenDiag.at(i) == 'K') {
            Utils::setBit(this->utils->ones[count], &this->pieces[1][5]);
            this->kingPos[1] = count;
            count++;
        } else if (isdigit(fenDiag.at(i))) {
            count += int(fenDiag.at(i)) - 48;
        }
    }

    this->trait = fenTrait == "w";
    this->pep = fenPep.at(0) == '-' ? -1 : (8 - (int(fenPep.at(1)) - 48)) * 8 + (int(fenPep.at(0)) - 97);

    this->shortCastles[1] = fenCastles.find('K') != string::npos;
    this->shortCastles[0] = fenCastles.find('k') != string::npos;
    this->longCastles[1] = fenCastles.find('Q') != string::npos;
    this->longCastles[0] = fenCastles.find('q') != string::npos;

    this->init();
    this->initZobrist();
}

void Board::init()
{
    this->white = this->pieces[1][0] | this->pieces[1][1] | this->pieces[1][2] | this->pieces[1][3] | this->pieces[1][4] | this->pieces[1][5];
    this->black = this->pieces[0][0] | this->pieces[0][1] | this->pieces[0][2] | this->pieces[0][3] | this->pieces[0][4] | this->pieces[0][5];
    this->board = this->white | this->black;
}

void Board::initZobrist()
{
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 12; ++j) {
            this->zobristBoard[i][j] = this->utils->getRand();
        }
    }
    this->zobristTrait[0] = this->utils->getRand();
    this->zobristTrait[1] = this->utils->getRand();

    for (int i = -1; i < 64; ++i) {
        this->zobristPep[i] = this->utils->getRand();
    }

    for (int i = 0; i < 4; ++i) {
        this->zobristCastle[i] = this->utils->getRand();
    }

    unsigned long long h = 0;
    for (int i = 0; i < 64; ++i) {
        if ((this->board & this->utils->ones[i]) == this->utils->ones[i]) {
            h ^= this->zobristBoard[i][this->getPieceAt(i)];
        }
    }

    h ^= this->zobristPep[this->pep];

    if (this->shortCastles[0]) {
        h ^= this->zobristCastle[0];
    }

    if (this->shortCastles[1]) {
        h ^= this->zobristCastle[1];
    }

    if (this->longCastles[0]) {
        h ^= this->zobristCastle[2];
    }

    if (this->longCastles[1]) {
        h ^= this->zobristCastle[3];
    }

    if (this->trait) {
        h ^= this->zobristTrait[0];
    } else {
        h ^= this->zobristTrait[1];
    }

    this->zobristKey = h;
}

int Board::getPieceAt(int i)
{
    if (Utils::isBit(this->utils->ones[i], this->pieces[0][0])) {
        return 0;
    } else if (Utils::isBit(this->utils->ones[i], this->pieces[0][1])) {
        return 1;
    } else if (Utils::isBit(this->utils->ones[i], this->pieces[0][2])) {
        return 2;
    } else if (Utils::isBit(this->utils->ones[i], this->pieces[0][3])) {
        return 3;
    } else if (Utils::isBit(this->utils->ones[i], this->pieces[0][4])) {
        return 4;
    } else if (Utils::isBit(this->utils->ones[i], this->pieces[0][5])) {
        return 5;
    } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][0])) {
        return 6;
    } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][1])) {
        return 7;
    } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][2])) {
        return 8;
    } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][3])) {
        return 9;
    } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][4])) {
        return 10;
    } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][5])) {
        return 11;
    } else {
        // pas censé arriver :s
        return -1;
    }
}

void Board::display()
{
    printf("Trait            : %s\n", this->trait ? "blanc" : "noir");
    cout << "Pions blancs     : ";
    cout << bitset<64>(this->pieces[1][0]);
    printf(" : %llu\n", this->pieces[1][0]);
    cout << "Cavaliers blancs : ";
    cout << bitset<64>(this->pieces[1][1]);
    printf(" : %llu\n", this->pieces[1][1]);
    cout << "Fous blancs      : ";
    cout << bitset<64>(this->pieces[1][2]);
    printf(" : %llu\n", this->pieces[1][2]);
    cout << "Tours blanches   : ";
    cout << bitset<64>(this->pieces[1][3]);
    printf(" : %llu\n", this->pieces[1][3]);
    cout << "Dames blanches   : ";
    cout << bitset<64>(this->pieces[1][4]);
    printf(" : %llu\n", this->pieces[1][4]);
    cout << "Roi blanc        : ";
    cout << bitset<64>(this->pieces[1][5]);
    printf(" : %llu\n", this->pieces[1][5]);
    cout << "Pions noirs      : ";
    cout << bitset<64>(this->pieces[0][0]);
    printf(" : %llu\n", this->pieces[0][0]);
    cout << "Cavaliers noirs  : ";
    cout << bitset<64>(this->pieces[0][1]);
    printf(" : %llu\n", this->pieces[0][1]);
    cout << "Fous noirs       : ";
    cout << bitset<64>(this->pieces[0][2]);
    printf(" : %llu\n", this->pieces[0][2]);
    cout << "Tours noires     : ";
    cout << bitset<64>(this->pieces[0][3]);
    printf(" : %llu\n", this->pieces[0][3]);
    cout << "Dames noires     : ";
    cout << bitset<64>(this->pieces[0][4]);
    printf(" : %llu\n", this->pieces[0][4]);
    cout << "Roi noir         : ";
    cout << bitset<64>(this->pieces[0][5]);
    printf(" : %llu\n", this->pieces[0][5]);
    cout << "Blancs           : ";
    cout << bitset<64>(this->white);
    printf(" : %llu\n", this->white);
    cout << "Noirs            : ";
    cout << bitset<64>(this->black);
    printf(" : %llu\n", this->black);
    cout << "Tout             : ";
    cout << bitset<64>(this->board);
    printf(" : %llu\n", this->board);
    cout << "Zobrist          : ";
    cout << bitset<64>(this->zobristKey);
    printf(" : %llu\n", this->zobristKey);
    cout << "Pep              : ";
    cout << this->pep << endl;
    cout << "Petits roques    : ";
    cout << this->shortCastles[0] << " " << this->shortCastles[1] << endl;
    cout << "Grands roques    : ";
    cout << this->longCastles[0] << " " << this->longCastles[1] << endl;
}

vector<vector<Move>> Board::getMoves()
{
    vector<vector<Move>> moves;
    for (int i = 0; i < 64; ++i) {
        if (Utils::isBit(this->utils->ones[i], this->pieces[this->trait][0])) {
            this->applyPawnMoves(i, &moves);
        }
        if (Utils::isBit(this->utils->ones[i], this->pieces[this->trait][1])) {
            this->applyKnightMoves(i, &moves);
        }
        if (Utils::isBit(this->utils->ones[i], this->pieces[this->trait][2])) {
            this->applyBishopMoves(i, &moves);
        }
        if (Utils::isBit(this->utils->ones[i], this->pieces[this->trait][3])) {
            this->applyRookMoves(i, &moves);
        }
        if (Utils::isBit(this->utils->ones[i], this->pieces[this->trait][4])) {
            this->applyQueenMoves(i, &moves);
        }
        if (Utils::isBit(this->utils->ones[i], this->pieces[this->trait][5])) {
            this->applyKingMoves(i, &moves);
        }
    }
    return moves;
}

void Board::applyMove(vector<Move> *v, vector<vector<Move>> *moves)
{
    this->makeMove(*v, true);
    if (!this->isInCheck()) {
        moves->push_back(*v);
    }
    this->makeMove(*v, false);
}

void Board::applyPawnMoves(int i, vector<vector<Move>> *moves)
{
    if (!Utils::isBit(this->utils->ones[i + this->utils->pawnMoves[this->trait]], this->board)) {
        if (Utils::isPawnPromote(this->trait, i)) {
            for (int k = 1; k < 5; k++) {
                vector<Move> v;
                v.emplace_back(0, this->trait, i, this->utils->ones[i], false);
                v.emplace_back(k, this->trait, i + this->utils->pawnMoves[this->trait], this->utils->ones[i + this->utils->pawnMoves[this->trait]], true);
                if (this->pep != -1) {
                    v.emplace_back("pep", this->pep, -1);

                }
                this->applyMove(&v, moves);
            }
        } else {
            vector<Move> v;
            v.emplace_back(0, this->trait, i, this->utils->ones[i], false);
            v.emplace_back(0, this->trait, i + this->utils->pawnMoves[this->trait], this->utils->ones[i + this->utils->pawnMoves[this->trait]], true);
            if (this->pep != -1) {
                v.emplace_back("pep", this->pep, -1);
            }
            this->applyMove(&v, moves);
        }

        if (Utils::isPawnFirstMove(this->trait, i)) {
            if (!Utils::isBit(this->utils->ones[i + this->utils->pawnMoves[this->trait] * 2], this->board)) {
                vector<Move> v;
                v.emplace_back(0, this->trait, i, this->utils->ones[i], false);
                v.emplace_back(0, this->trait, i + this->utils->pawnMoves[this->trait] * 2, this->utils->ones[i + this->utils->pawnMoves[this->trait] * 2], true);
                v.emplace_back("pep", this->pep, i + this->utils->pawnMoves[this->trait]);
                this->applyMove(&v, moves);
            }
        }
    }

    for (int j = 0 ; j < this->utils->pawnCapturesCachePosition[this->trait][i].size(); ++j) {
        if (Utils::isBit(this->utils->pawnCapturesCachePosition[this->trait][i][j], this->trait ? this->black : this->white)) {
            if (Utils::isPawnPromote(this->trait, i)) {
                for (int k = 1; k < 5; ++k) {
                    vector<Move> v;
                    v.emplace_back(0, this->trait, i, this->utils->ones[i], false);
                    v.emplace_back(k, this->trait, this->utils->pawnCapturesCache[this->trait][i][j], this->utils->pawnCapturesCachePosition[this->trait][i][j], true);
                    if (this->pep != -1) {
                        v.emplace_back("pep", this->pep, -1);
                    }
                    v.emplace_back(\
                        this->getPieceAt(this->utils->pawnCapturesCache[this->trait][i][j]) % 6,\
                        !this->trait,\
                        this->utils->pawnCapturesCache[this->trait][i][j],\
                        this->utils->pawnCapturesCachePosition[this->trait][i][j],\
                        false\
                    );
                    this->applyMove(&v, moves);
                }
            } else {
                vector<Move> v;
                v.emplace_back(0, this->trait, i, this->utils->ones[i], false);
                v.emplace_back(0, this->trait, this->utils->pawnCapturesCache[this->trait][i][j], this->utils->pawnCapturesCachePosition[this->trait][i][j], true);
                if (this->pep != -1) {
                    v.emplace_back("pep", this->pep, -1);
                }
                v.emplace_back(\
                    this->getPieceAt(this->utils->pawnCapturesCache[this->trait][i][j]) % 6,\
                    !this->trait,\
                    this->utils->pawnCapturesCache[this->trait][i][j],
                    this->utils->pawnCapturesCachePosition[this->trait][i][j],\
                    false\
                );
                this->applyMove(&v, moves);
            }
        }
        if (this->utils->pawnCapturesCache[this->trait][i][j] == this->pep) {
            vector<Move> v;
            v.emplace_back(0, this->trait, i, this->utils->ones[i], 0);
            v.emplace_back(0, this->trait, this->utils->pawnCapturesCache[this->trait][i][j], this->utils->pawnCapturesCachePosition[this->trait][i][j], 1);
            v.emplace_back("pep", this->pep, -1);
            v.emplace_back(\
                0,\
                !this->trait,\
                this->utils->pawnCapturesCache[this->trait][i][j] + 8 * this->trait,\
                this->trait ? this->utils->pawnCapturesCachePosition[this->trait][i][j] << 8 : this->utils->pawnCapturesCachePosition[this->trait][i][j] >> 8,\
                false\
            );
            this->applyMove(&v, moves);
        }
    }
}

void Board::applyKnightMoves(int i, vector<vector<Move>> *moves)
{
    for (int j = 0 ; j < this->utils->knightMovesCachePosition[i].size(); ++j) {
        if (!Utils::isBit(this->utils->knightMovesCachePosition[i][j], this->trait ? this->white : this->black)) {
            vector<Move> v;
            v.emplace_back(1, this->trait, i, this->utils->ones[i], false);
            v.emplace_back(1, this->trait, this->utils->knightMovesCache[i][j], this->utils->knightMovesCachePosition[i][j], true);
            if (this->pep != -1) {
                v.emplace_back("pep", this->pep, -1);
            }
            if (((this->trait ? this->black : this->white) & this->utils->knightMovesCachePosition[i][j]) == this->utils->knightMovesCachePosition[i][j]) {
                v.emplace_back(\
                    this->getPieceAt(this->utils->knightMovesCache[i][j]) % 6,\
                    !this->trait,\
                    this->utils->knightMovesCache[i][j],\
                    this->utils->knightMovesCachePosition[i][j],\
                    false\
                );
            }
            this->applyMove(&v, moves);
        }
    }
}

void Board::applyBishopMoves(int i, vector<vector<Move>> *moves)
{
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < this->utils->bishopMovesCachePosition[i][j].size(); ++k) {
            if (!Utils::isBit(this->utils->bishopMovesCachePosition[i][j][k], this->trait ? this->white : this->black)) {
                vector<Move> v;
                v.emplace_back(2, this->trait, i, this->utils->ones[i], false);
                v.emplace_back(2, this->trait, this->utils->bishopMovesCache[i][j][k], this->utils->bishopMovesCachePosition[i][j][k], true);
                if (this->pep != -1) {
                    v.emplace_back("pep", this->pep, -1);
                }
                if (Utils::isBit(this->utils->bishopMovesCachePosition[i][j][k], this->trait ? this->black : this->white)) {
                    v.emplace_back(\
                        this->getPieceAt(this->utils->bishopMovesCache[i][j][k]) % 6,\
                        !this->trait,\
                        this->utils->bishopMovesCache[i][j][k],\
                        this->utils->bishopMovesCachePosition[i][j][k],\
                        false\
                    );
                    this->applyMove(&v, moves);
                    break;
                } else {
                    this->applyMove(&v, moves);
                }
            } else {
                break;
            }
        }
    }
}

void Board::applyRookMoves(int i, vector<vector<Move>> *moves)
{
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < this->utils->rookMovesCachePosition[i][j].size(); ++k) {
            if (!Utils::isBit(this->utils->rookMovesCachePosition[i][j][k], this->trait ? this->white : this->black)) {
                vector<Move> v;
                v.emplace_back(3, this->trait, i, this->utils->ones[i], false);
                v.emplace_back(3, this->trait, this->utils->rookMovesCache[i][j][k], this->utils->rookMovesCachePosition[i][j][k], true);
                if (this->pep != -1) {
                    v.emplace_back("pep", this->pep, -1);
                }
                if (Utils::isBit(this->utils->rookMovesCachePosition[i][j][k], this->trait ? this->black : this->white)) {
                    v.emplace_back(\
                        this->getPieceAt(this->utils->rookMovesCache[i][j][k]) % 6,\
                        !this->trait,\
                        this->utils->rookMovesCache[i][j][k],\
                        this->utils->rookMovesCachePosition[i][j][k],\
                        false\
                    );
                    if (this->shortCastles[this->trait] && (i == (this->trait ? 63 : 7))) {
                        v.emplace_back("castle", this->trait, false);
                    }
                    if (this->longCastles[this->trait] && (i == (this->trait ? 56 : 0))) {
                        v.emplace_back("castle", this->trait, true);
                    }
                    this->applyMove(&v, moves);
                    break;
                } else {
                    if (this->shortCastles[this->trait] && (i == (this->trait ? 63 : 7))) {
                        v.emplace_back("castle", this->trait, false);
                    }
                    if (this->longCastles[this->trait] && (i == (this->trait ? 56 : 0))) {
                        v.emplace_back("castle", this->trait, true);
                    }
                    this->applyMove(&v, moves);
                }
            } else {
                break;
            }
        }
    }
}

void Board::applyQueenMoves(int i, vector<vector<Move>> *moves)
{
    for (int j = 0; j < 8; ++j) {
        for (int k = 0; k < this->utils->queenMovesCachePosition[i][j].size(); ++k) {
            if (!Utils::isBit(this->utils->queenMovesCachePosition[i][j][k], this->trait ? this->white : this->black)) {
                vector<Move> v;
                v.emplace_back(4, this->trait, i, this->utils->ones[i], false);
                v.emplace_back(4, this->trait, this->utils->queenMovesCache[i][j][k], this->utils->queenMovesCachePosition[i][j][k], true);

                if (this->pep != -1) {
                    v.emplace_back("pep", this->pep, -1);
                }
                if (Utils::isBit(this->utils->queenMovesCachePosition[i][j][k], this->trait ? this->black : this->white)) {
                    v.emplace_back(\
                        this->getPieceAt(this->utils->queenMovesCache[i][j][k]) % 6,\
                        !this->trait,\
                        this->utils->queenMovesCache[i][j][k],\
                        this->utils->queenMovesCachePosition[i][j][k],\
                        false\
                    );
                    this->applyMove(&v, moves);
                    break;
                } else {
                    this->applyMove(&v, moves);
                }
            } else {
                break;
            }
        }
    }
}

void Board::applyKingMoves(int i, vector<vector<Move>> *moves)
{
    for (int j = 0 ; j < this->utils->kingMovesCachePosition[i].size(); ++j) {
        if (!Utils::isBit(this->utils->kingMovesCachePosition[i][j], this->trait ? this->white : this->black)) {
            vector<Move> v;
            v.emplace_back(5, this->trait, i, this->utils->ones[i], false);
            v.emplace_back(5, this->trait, this->utils->kingMovesCache[i][j], this->utils->kingMovesCachePosition[i][j], true);
            if (this->pep != -1) {
                v.emplace_back("pep", this->pep, -1);
            }
            v.emplace_back("kingpos", i, this->utils->kingMovesCache[i][j], this->trait);
            if (this->shortCastles[this->trait]) {
                v.emplace_back("castle", this->trait, false);
            }
            if (this->longCastles[this->trait]) {
                v.emplace_back("castle", this->trait, true);
            }
            if (Utils::isBit(this->utils->kingMovesCachePosition[i][j], this->trait ? this->black : this->white)) {
                v.emplace_back(\
                    this->getPieceAt(this->utils->kingMovesCache[i][j]) % 6,\
                    !this->trait,\
                    this->utils->kingMovesCache[i][j],\
                    this->utils->kingMovesCachePosition[i][j],\
                    false\
                );
            }
            this->applyMove(&v, moves);
        }
    }

    if (!this->isInCheck()) {
        if (this->shortCastles[this->trait] && ((~this->utils->shortCastlesCache[this->trait] | this->board) == ~this->utils->shortCastlesCache[this->trait])) {
            int firstCache = this->trait ? 61 : 5;
            unsigned long long firstCachePosition = this->utils->ones[firstCache];
            vector<Move> v1;
            v1.emplace_back("kingpos", i, firstCache, this->trait);
            this->makeMove(v1, true);
            if (!this->isInCheck()) {
                this->makeMove(v1, false);
                int secondCache = this->trait ? 62 : 6;
                unsigned long long secondCachePosition = this->utils->ones[secondCache];
                vector<Move> v2;
                v2.emplace_back("kingpos", i, secondCache, this->trait);
                this->makeMove(v2, true);
                if (!this->isInCheck()) {
                    this->makeMove(v2, false);
                    vector<Move> shortCastle;
                    shortCastle.emplace_back(5, this->trait, i, this->utils->ones[i], false);
                    shortCastle.emplace_back(5, this->trait, secondCache, secondCachePosition, true);
                    shortCastle.emplace_back(3, this->trait, this->trait ? 63 : 7,
                                             this->utils->ones[this->trait ? 63 : 7], false);
                    shortCastle.emplace_back(3, this->trait, firstCache, firstCachePosition, true);
                    shortCastle.emplace_back("kingpos", i, secondCache, this->trait);
                    shortCastle.emplace_back("castle", this->trait, false);
                    shortCastle.emplace_back("castle", this->trait, true);
                    if (this->pep != -1) {
                        shortCastle.emplace_back("pep", this->pep, -1);
                    }
                    this->applyMove(&shortCastle, moves);
                } else {
                    this->makeMove(v2, false);
                }
            } else {
                this->makeMove(v1, false);
            }
        }

        if (this->longCastles[this->trait] && ((~this->utils->longCastlesCache[this->trait] | this->board) ==
                                               ~this->utils->longCastlesCache[this->trait])) {
            int firstCache = this->trait ? 59 : 3;
            unsigned long long firstCachePosition = this->trait ? this->utils->ones[59] : this->utils->ones[3];
            vector<Move> v1;
            v1.emplace_back("kingpos", i, firstCache, this->trait);
            this->makeMove(v1, true);
            if (!this->isInCheck()) {
                this->makeMove(v1, false);
                int secondCache = this->trait ? 58 : 2;
                unsigned long long secondCachePosition = this->utils->ones[secondCache];
                vector<Move> v2;
                v2.emplace_back("kingpos", i, secondCache, this->trait);
                this->makeMove(v2, true);
                if (!this->isInCheck()) {
                    this->makeMove(v2, false);
                    vector<Move> longCastle;
                    longCastle.emplace_back(5, this->trait, i, this->utils->ones[i], false);
                    longCastle.emplace_back(5, this->trait, secondCache, secondCachePosition, true);
                    longCastle.emplace_back(3, this->trait, this->trait ? 56 : 0,
                                            this->utils->ones[this->trait ? 56 : 0], false);
                    longCastle.emplace_back(3, this->trait, firstCache, firstCachePosition, true);
                    longCastle.emplace_back("kingpos", i, secondCache, this->trait);

                    longCastle.emplace_back("castle", this->trait, false);
                    longCastle.emplace_back("castle", this->trait, true);
                    if (this->pep != -1) {
                        longCastle.emplace_back("pep", this->pep, -1);
                    }
                    this->applyMove(&longCastle, moves);
                } else {
                    this->makeMove(v2, false);
                }
            } else {
                this->makeMove(v1, false);
            }
        }
    }
}

void Board::makeMove(vector<Move> moves, bool doit)
{
    for (int i = 0; i < moves.size(); ++i) {
        if (moves[i].type == "pep") {
            this->pep = doit ? moves[i].pep[1] : moves[i].pep[0];
            this->zobristKey ^= this->zobristPep[moves[i].pep[1]];
            this->zobristKey ^= this->zobristPep[moves[i].pep[0]];
        } else if (moves[i].type == "kingpos") {
            this->kingPos[moves[i].trait] = doit ? moves[i].king[1] : moves[i].king[0];
        } else if (moves[i].type == "castle") {
            if(doit) {
                if (!moves[i].side) {
                    this->shortCastles[moves[i].trait] = false;
                    this->zobristKey ^= this->zobristCastle[moves[i].trait?1:0];
                } else {
                    this->longCastles[moves[i].trait] = false;
                    this->zobristKey ^= this->zobristCastle[moves[i].trait?3:2];
                }
            } else {
                if (!moves[i].side) {
                    this->shortCastles[moves[i].trait] = true;
                    this->zobristKey ^= this->zobristCastle[moves[i].trait?1:0];
                } else {
                    this->longCastles[moves[i].trait] = true;
                    this->zobristKey ^= this->zobristCastle[moves[i].trait?3:2];
                }
            }
        } else if (doit) {
            if (moves[i].doit) {
                Utils::setBit(moves[i].value, &this->pieces[moves[i].trait][moves[i].pieceType]);
            } else {
                Utils::unsetBit(moves[i].value, &this->pieces[moves[i].trait][moves[i].pieceType]);
            }
            this->zobristKey ^= this->zobristBoard[moves[i].position][(moves[i].trait*6)+moves[i].pieceType];
        } else {
            if (moves[i].doit) {
                Utils::unsetBit(moves[i].value, &this->pieces[moves[i].trait][moves[i].pieceType]);
            } else {
                Utils::setBit(moves[i].value, &this->pieces[moves[i].trait][moves[i].pieceType]);
            }
            this->zobristKey ^= this->zobristBoard[moves[i].position][(moves[i].trait*6)+moves[i].pieceType];
        }
    }
    this->trait = !this->trait;
    this->zobristKey ^= this->zobristTrait[this->trait];
    this->init();
}

bool Board::isInCheck()
{
    for (int i = 0; i < this->utils->pawnCapturesCachePosition[!this->trait][this->kingPos[!this->trait]].size(); ++i) {
        if (Utils::isBit(this->utils->pawnCapturesCachePosition[!this->trait][this->kingPos[!this->trait]][i], this->pieces[this->trait][0])) {
            return true;
        }
    }

    for (int i = 0; i < this->utils->knightMovesCachePosition[this->kingPos[!this->trait]].size(); ++i) {
        if (Utils::isBit(this->utils->knightMovesCachePosition[this->kingPos[!this->trait]][i], this->pieces[this->trait][1])) {
            return true;
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < this->utils->bishopMovesCachePosition[this->kingPos[!this->trait]][i].size(); ++j) {
            if (Utils::isBit(this->utils->bishopMovesCachePosition[this->kingPos[!this->trait]][i][j], this->pieces[this->trait][2])
             || Utils::isBit(this->utils->bishopMovesCachePosition[this->kingPos[!this->trait]][i][j], this->pieces[this->trait][4])
             || ((j == 0) && Utils::isBit(this->utils->bishopMovesCachePosition[this->kingPos[!this->trait]][i][j], this->pieces[this->trait][5]))
            ) {
                return true;
            }
            if (Utils::isBit(this->utils->bishopMovesCachePosition[this->kingPos[!this->trait]][i][j], board)) {
                break;
            }
        }
        for (int j = 0; j < this->utils->rookMovesCachePosition[this->kingPos[!this->trait]][i].size(); ++j) {
            if (Utils::isBit(this->utils->rookMovesCachePosition[this->kingPos[!this->trait]][i][j], this->pieces[this->trait][3])
             || Utils::isBit(this->utils->rookMovesCachePosition[this->kingPos[!this->trait]][i][j], this->pieces[this->trait][4])
             || ((j == 0) && Utils::isBit(this->utils->rookMovesCachePosition[this->kingPos[!this->trait]][i][j], this->pieces[this->trait][5]))
            ) {
                return true;
            }
            if (Utils::isBit(this->utils->rookMovesCachePosition[this->kingPos[!this->trait]][i][j], board)) {
                break;
            }
        }
    }

    return false;
}

string Board::getFen() {
    string result = "";
    int j = 0;
    int vide = 0;
    for (int i = 0; i < 64; ++i) {
        if (Utils::isBit(this->utils->ones[i], this->pieces[0][0])) {
            result += "p";
        } else if (Utils::isBit(this->utils->ones[i], this->pieces[0][1])) {
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            result += "n";
        } else if (Utils::isBit(this->utils->ones[i], this->pieces[0][2])) {
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            result += "b";
        } else if (Utils::isBit(this->utils->ones[i], this->pieces[0][3])) {
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            result += "r";
        } else if (Utils::isBit(this->utils->ones[i], this->pieces[0][4])) {
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            result += "q";

        } else if (Utils::isBit(this->utils->ones[i], this->pieces[0][5])) {
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            result += "k";
        } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][0])) {
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            result += "P";
        } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][1])) {
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            result += "N";
        } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][2])) {
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            result += "B";
        } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][3])) {
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            result += "R";
        } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][4])) {
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            result += "Q";

        } else if (Utils::isBit(this->utils->ones[i], this->pieces[1][5])) {
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            result += "K";
        } else {
            vide++;
        }

        j++;
        if (j == 8) {
            j = 0;
            if (vide != 0) {
                result += to_string(vide);
                vide = 0;
            }
            if (i != 63) {
                result += "/";
            }
        }
    }
    result += " ";
    if (this->trait) {
        result += "w";
    } else {
        result += "b";
    }
    result += " ";

    bool castle = false;
    if (this->shortCastles[1]) {
        result += "K";
        castle = true;
    }
    if (this->longCastles[1]) {
        result += "Q";
        castle = true;
    }
    if (this->shortCastles[0]) {
        result += "k";
        castle = true;
    }
    if (this->longCastles[0]) {
        result += "q";
        castle = true;
    }
    if (!castle) {
        result += "-";
    }
    result += " ";
    if (this->pep == -1) {
        result += "-";
    } else {
        result += to_string(this->pep);
    }
    return result;
}