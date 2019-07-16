#include "move.h"

Move::Move(int pieceType, bool trait, int position, unsigned long long value, bool doit)
{
    this->pieceType = pieceType;
    this->trait = trait;
    this->position = position;
    this->value = value;
    this->doit = doit;
}

Move::Move(std::string type, int prev, int next)
{
    this->type = type;
    this->pep[0] = prev;
    this->pep[1] = next;
}

Move::Move(std::string type, int prev, int next, bool trait)
{
    this->type = type;
    this->trait = trait;
    this->king[0] = prev;
    this->king[1] = next;
}

Move::Move(std::string type, bool trait, bool side)
{
    this->type = type;
    this->trait = trait;
    this->side = side;
}

