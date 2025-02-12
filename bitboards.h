#ifndef BITBOARDS_H_
#define BITBOARDS_H_

// piece bitboards
U64 bitboards[12];

// define occupancy bitboards
U64 occupancies[3];

// side to move
int side;

// enpassant square
int enpassant = no_sq;

// define castling rights
int castle;


#endif