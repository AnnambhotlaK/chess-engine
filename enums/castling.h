#ifndef CASTLING_H_
#define CASTLING_H_

typedef enum
{
    wk = 1,
    wq = 2,
    bk = 4,
    bq = 8,
} Castling;

extern Castling CastlingRights;

#endif